/*
 * mm.c - Segregated explicit free list allocator with footer elision
 *         for allocated blocks.
 *
 * Optimizations over baseline:
 *  - realloc: in-place expand into next free / epilogue-sbrk / prev merge
 *  - place: large-remainder splits put alloc block at END (benefits realloc)
 *  - find_fit: best-fit in starting class, first-fit for larger classes
 *  - inline hot helpers
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
    "2team",
    "Nam",
    "ehdgus5178@gmail.com",
    "",
    ""
};

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 7)
#define LISTLIMIT 20
#define MIN_ALLOC_BLOCK_SIZE 8
#define MIN_FREE_BLOCK_SIZE 16

#define ALLOC_MASK 0x1
#define PREV_ALLOC_MASK 0x2

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PACK(size, prev_alloc, alloc) ((size) | ((prev_alloc) ? PREV_ALLOC_MASK : 0) | ((alloc) ? ALLOC_MASK : 0))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & ALLOC_MASK)
#define GET_PREV_ALLOC(p) ((GET(p) & PREV_ALLOC_MASK) >> 1)

#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define PRED_FIELD(bp) ((char *)(bp))
#define SUCC_FIELD(bp) ((char *)(bp) + WSIZE)

#define PTR_TO_OFF(ptr) ((ptr) == NULL ? 0U : (unsigned int)((char *)(ptr) - (char *)mem_heap_lo()))
#define OFF_TO_PTR(off) ((off) == 0 ? NULL : (char *)mem_heap_lo() + (off))

#define SET_PRED(bp, ptr) PUT(PRED_FIELD(bp), PTR_TO_OFF(ptr))
#define SET_SUCC(bp, ptr) PUT(SUCC_FIELD(bp), PTR_TO_OFF(ptr))
#define PRED(bp) OFF_TO_PTR(GET(PRED_FIELD(bp)))
#define SUCC(bp) OFF_TO_PTR(GET(SUCC_FIELD(bp)))

static char *heap_listp = NULL;
static char *seg_free_lists[LISTLIMIT];

/* Forward declarations */
static inline size_t adjust_block_size(size_t size);
static inline int get_list_index(size_t size);
static inline void set_next_prev_alloc(void *bp, size_t prev_alloc);
static inline void write_alloc_block(void *bp, size_t size, size_t prev_alloc);
static inline void write_free_block(void *bp, size_t size, size_t prev_alloc);
static void insert_block(void *bp, size_t size);
static void remove_block(void *bp);
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void *place(void *bp, size_t asize);

/* ================================================================
 *  Helpers
 * ================================================================ */

static inline size_t adjust_block_size(size_t size)
{
    size_t asize;

    if (size == 0)
        return 0;

    asize = ALIGN(size + WSIZE);
    if (asize < MIN_ALLOC_BLOCK_SIZE)
        asize = MIN_ALLOC_BLOCK_SIZE;

    return asize;
}

static inline int get_list_index(size_t size)
{
    int index = 0;
    size_t bound = MIN_FREE_BLOCK_SIZE;

    while (index < LISTLIMIT - 1 && size > bound) {
        bound <<= 1;
        index++;
    }

    return index;
}

static inline void set_next_prev_alloc(void *bp, size_t prev_alloc)
{
    char *next_hdr = HDRP(NEXT_BLKP(bp));
    unsigned int value = GET(next_hdr);

    if (prev_alloc)
        value |= PREV_ALLOC_MASK;
    else
        value &= ~PREV_ALLOC_MASK;

    PUT(next_hdr, value);
}

static inline void write_alloc_block(void *bp, size_t size, size_t prev_alloc)
{
    PUT(HDRP(bp), PACK(size, prev_alloc, 1));
    set_next_prev_alloc(bp, 1);
}

static inline void write_free_block(void *bp, size_t size, size_t prev_alloc)
{
    PUT(HDRP(bp), PACK(size, prev_alloc, 0));
    PUT(FTRP(bp), PACK(size, prev_alloc, 0));
    SET_PRED(bp, NULL);
    SET_SUCC(bp, NULL);
    set_next_prev_alloc(bp, 0);
}

/* ================================================================
 *  Segregated list operations (size-ordered)
 * ================================================================ */

static void insert_block(void *bp, size_t size)
{
    int index = get_list_index(size);
    char *search = seg_free_lists[index];
    char *insert = NULL;

    while (search != NULL && size > GET_SIZE(HDRP(search))) {
        insert = search;
        search = SUCC(search);
    }

    if (insert == NULL) {
        SET_PRED(bp, NULL);
        SET_SUCC(bp, search);
        if (search != NULL)
            SET_PRED(search, bp);
        seg_free_lists[index] = bp;
    } else {
        SET_SUCC(insert, bp);
        SET_PRED(bp, insert);
        SET_SUCC(bp, search);
        if (search != NULL)
            SET_PRED(search, bp);
    }
}

static void remove_block(void *bp)
{
    int index = get_list_index(GET_SIZE(HDRP(bp)));
    char *pred = PRED(bp);
    char *succ = SUCC(bp);

    if (pred != NULL)
        SET_SUCC(pred, succ);
    else
        seg_free_lists[index] = succ;

    if (succ != NULL)
        SET_PRED(succ, pred);
}

/* ================================================================
 *  Core allocator
 * ================================================================ */

int mm_init(void)
{
    int i;

    for (i = 0; i < LISTLIMIT; i++)
        seg_free_lists[i] = NULL;

    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;

    PUT(heap_listp, 0);
    PUT(heap_listp + WSIZE, PACK(DSIZE, 1, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1, 1));
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1, 1));
    heap_listp += (2 * WSIZE);

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;

    return 0;
}

static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;
    size_t prev_alloc;

    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if (size < MIN_FREE_BLOCK_SIZE)
        size = MIN_FREE_BLOCK_SIZE;

    if ((bp = mem_sbrk((int)size)) == (void *)-1)
        return NULL;

    prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    write_free_block(bp, size, prev_alloc);
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 0, 1));

    return coalesce(bp);
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    void *prevbp;
    void *nextbp;

    if (prev_alloc && next_alloc) {
        insert_block(bp, size);
        return bp;
    }

    if (prev_alloc && !next_alloc) {
        nextbp = NEXT_BLKP(bp);
        remove_block(nextbp);
        size += GET_SIZE(HDRP(nextbp));
        write_free_block(bp, size, prev_alloc);
        insert_block(bp, size);
        return bp;
    }

    if (!prev_alloc && next_alloc) {
        prevbp = PREV_BLKP(bp);
        remove_block(prevbp);
        size += GET_SIZE(HDRP(prevbp));
        write_free_block(prevbp, size, GET_PREV_ALLOC(HDRP(prevbp)));
        insert_block(prevbp, size);
        return prevbp;
    }

    prevbp = PREV_BLKP(bp);
    nextbp = NEXT_BLKP(bp);
    remove_block(prevbp);
    remove_block(nextbp);
    size += GET_SIZE(HDRP(prevbp)) + GET_SIZE(HDRP(nextbp));
    write_free_block(prevbp, size, GET_PREV_ALLOC(HDRP(prevbp)));
    insert_block(prevbp, size);
    return prevbp;
}

/* ================================================================
 *  find_fit: best-fit in the starting size class (already sorted),
 *            first-fit for larger classes.
 * ================================================================ */
static void *find_fit(size_t asize)
{
    void *bp;
    void *best = NULL;
    size_t best_size = (size_t)-1;
    size_t block_size;
    int index = get_list_index(asize);
    int start_index = index;
    int global_best = (asize <= 160);

    for (; index < LISTLIMIT; index++) {
        bp = seg_free_lists[index];
        if (index == start_index || global_best) {
            /* Best-fit within the starting class */
            while (bp != NULL) {
                block_size = GET_SIZE(HDRP(bp));
                if (block_size >= asize) {
                    if (block_size == asize)
                        return bp;
                    if (block_size < best_size) {
                        best = bp;
                        best_size = block_size;
                    }
                }
                bp = SUCC(bp);
            }
            if (best != NULL)
                return best;
        } else {
            /* First-fit for larger classes — first block works */
            if (bp != NULL)
                return bp;
        }
    }

    return NULL;
}

/* ================================================================
 *  place: split block. For large remainders (>= 96 bytes),
 *         put the allocated part at the END so the free remainder
 *         stays at the front (benefits realloc-heavy traces where
 *         blocks grow — the alloc block ends up near heap end).
 *         Returns the pointer to the allocated block payload.
 * ================================================================ */
#define SPLIT_THRESHOLD 96

static void *place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    size_t remain = csize - asize;
    size_t prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    void *nextbp;

    remove_block(bp);

    if (remain >= MIN_FREE_BLOCK_SIZE) {
        if (remain >= SPLIT_THRESHOLD && asize >= SPLIT_THRESHOLD) {
            /* Free block at front, alloc at end */
            write_free_block(bp, remain, prev_alloc);
            insert_block(bp, remain);
            nextbp = NEXT_BLKP(bp);
            write_alloc_block(nextbp, asize, 0);
            return nextbp;
        }
        /* Normal: alloc at front, free at back */
        write_alloc_block(bp, asize, prev_alloc);
        nextbp = NEXT_BLKP(bp);
        write_free_block(nextbp, remain, 1);
        insert_block(nextbp, remain);
        return bp;
    }

    write_alloc_block(bp, csize, prev_alloc);
    return bp;
}

void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    if (size == 0)
        return NULL;

    asize = adjust_block_size(size);
    if ((bp = find_fit(asize)) != NULL) {
        bp = place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE);
    if (asize >= 400 && asize <= 520)
        extendsize = MAX(asize << 2, CHUNKSIZE);
    else if (asize >= 120 && asize < 400)
        extendsize = MAX(asize << 1, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;

    bp = place(bp, asize);
    return bp;
}

void mm_free(void *ptr)
{
    size_t size;
    size_t prev_alloc;

    if (ptr == NULL)
        return;

    size = GET_SIZE(HDRP(ptr));
    prev_alloc = GET_PREV_ALLOC(HDRP(ptr));
    write_free_block(ptr, size, prev_alloc);
    coalesce(ptr);
}

/* ================================================================
 *  mm_realloc — optimized in-place strategies
 *
 *  Priority:
 *    1. Shrink in place
 *    2. Expand into next free block
 *    3. Expand via sbrk if at heap end (next is epilogue)
 *    4. Next free + sbrk combined
 *    5. Merge with previous free block (memmove required)
 *    6. Fallback: malloc + memcpy + free
 * ================================================================ */
void *mm_realloc(void *ptr, size_t size)
{
    size_t asize;
    size_t oldsize;
    size_t nextsize;
    size_t totalsize;
    size_t remain;
    size_t copySize;
    size_t prev_alloc;
    size_t extend;
    size_t prevsize;
    void *prevbp;
    void *nextbp;
    void *newptr;
    void *after_next;

    if (ptr == NULL)
        return mm_malloc(size);

    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    asize = adjust_block_size(size);
    oldsize = GET_SIZE(HDRP(ptr));

    /* ── Case 1: already big enough ─────────────────────── */
    if (oldsize >= asize) {
        remain = oldsize - asize;
        if (remain >= MIN_FREE_BLOCK_SIZE) {
            prev_alloc = GET_PREV_ALLOC(HDRP(ptr));
            write_alloc_block(ptr, asize, prev_alloc);
            nextbp = NEXT_BLKP(ptr);
            write_free_block(nextbp, remain, 1);
            coalesce(nextbp);
        }
        return ptr;
    }

    nextbp = NEXT_BLKP(ptr);
    nextsize = GET_SIZE(HDRP(nextbp));

    /* ── Case 2: expand into next free block ────────────── */
    if (!GET_ALLOC(HDRP(nextbp))) {
        totalsize = oldsize + nextsize;
        if (totalsize >= asize) {
            prev_alloc = GET_PREV_ALLOC(HDRP(ptr));
            remove_block(nextbp);
            remain = totalsize - asize;

            if (remain >= MIN_FREE_BLOCK_SIZE) {
                write_alloc_block(ptr, asize, prev_alloc);
                nextbp = NEXT_BLKP(ptr);
                write_free_block(nextbp, remain, 1);
                insert_block(nextbp, remain);
            } else {
                write_alloc_block(ptr, totalsize, prev_alloc);
            }
            return ptr;
        }
    }

    /* ── Case 3: at heap end (epilogue), just sbrk ──────── */
    if (nextsize == 0) {
        extend = asize - oldsize;
        extend = ALIGN(extend);
        if (extend < MIN_FREE_BLOCK_SIZE)
            extend = ALIGN(MIN_FREE_BLOCK_SIZE);

        if (mem_sbrk((int)extend) != (void *)-1) {
            prev_alloc = GET_PREV_ALLOC(HDRP(ptr));
            totalsize = oldsize + extend;
            remain = totalsize - asize;
            if (remain >= MIN_FREE_BLOCK_SIZE) {
                write_alloc_block(ptr, asize, prev_alloc);
                nextbp = NEXT_BLKP(ptr);
                write_free_block(nextbp, remain, 1);
                insert_block(nextbp, remain);
                PUT(HDRP(NEXT_BLKP(nextbp)), PACK(0, 0, 1));
            } else {
                write_alloc_block(ptr, totalsize, prev_alloc);
                PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1, 1));
            }
            return ptr;
        }
    }

    /* ── Case 4: next free + epilogue sbrk combined ─────── */
    if (!GET_ALLOC(HDRP(nextbp))) {
        after_next = NEXT_BLKP(nextbp);
        if (GET_SIZE(HDRP(after_next)) == 0) {
            totalsize = oldsize + nextsize;
            extend = asize - totalsize;
            extend = ALIGN(extend);
            if (extend < MIN_FREE_BLOCK_SIZE)
                extend = ALIGN(MIN_FREE_BLOCK_SIZE);

            if (mem_sbrk((int)extend) != (void *)-1) {
                remove_block(nextbp);
                prev_alloc = GET_PREV_ALLOC(HDRP(ptr));
                totalsize += extend;
                remain = totalsize - asize;
                if (remain >= MIN_FREE_BLOCK_SIZE) {
                    write_alloc_block(ptr, asize, prev_alloc);
                    nextbp = NEXT_BLKP(ptr);
                    write_free_block(nextbp, remain, 1);
                    insert_block(nextbp, remain);
                    PUT(HDRP(NEXT_BLKP(nextbp)), PACK(0, 0, 1));
                } else {
                    write_alloc_block(ptr, totalsize, prev_alloc);
                    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1, 1));
                }
                return ptr;
            }
        }
    }

    /* ── Case 5: merge with previous free block ─────────── */
    if (!GET_PREV_ALLOC(HDRP(ptr))) {
        prevbp = PREV_BLKP(ptr);
        prevsize = GET_SIZE(HDRP(prevbp));
        totalsize = prevsize + oldsize;

        /* Also grab next if free */
        if (!GET_ALLOC(HDRP(nextbp))) {
            totalsize += GET_SIZE(HDRP(nextbp));
        }

        if (totalsize >= asize) {
            remove_block(prevbp);
            if (!GET_ALLOC(HDRP(nextbp))) {
                remove_block(nextbp);
            }

            copySize = oldsize - WSIZE;
            memmove(prevbp, ptr, copySize);

            remain = totalsize - asize;
            if (remain >= MIN_FREE_BLOCK_SIZE) {
                write_alloc_block(prevbp, asize, GET_PREV_ALLOC(HDRP(prevbp)));
                nextbp = NEXT_BLKP(prevbp);
                write_free_block(nextbp, remain, 1);
                coalesce(nextbp);
            } else {
                write_alloc_block(prevbp, totalsize, GET_PREV_ALLOC(HDRP(prevbp)));
            }
            return prevbp;
        }
    }

    /* ── Case 6: fallback malloc + memcpy + free ────────── */
    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;

    copySize = oldsize - WSIZE;
    if (size < copySize)
        copySize = size;
    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    return newptr;
}
