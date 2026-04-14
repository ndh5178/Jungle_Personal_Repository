/*
 * mm-naive.c - 가장 빠르지만 메모리 효율은 가장 떨어지는 malloc 패키지.
 *
 * 이 단순한 구현에서는 brk 포인터를 단순히 증가시키는 방식으로 블록을
 * 할당합니다. 각 블록은 순수 payload만 가지며, 헤더와 푸터가 없습니다.
 * 블록은 병합(coalescing)되거나 재사용되지 않으며, realloc 역시
 * mm_malloc과 mm_free를 직접 이용해 구현되어 있습니다.
 *
 * 학생 안내: 이 헤더 주석은 본인이 구현한 방식의 핵심 아이디어를 설명하는
 * 주석으로 바꿔 주세요.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * 학생 안내: 다른 작업을 시작하기 전에 아래 구조체에
 * 팀 정보를 먼저 입력하세요.
 ********************************************************/
team_t team = {
    /* 팀 이름 */
    "2team",
    /* 첫 번째 팀원의 이름 */
    "Nam",
    /* 첫 번째 팀원의 이메일 주소 */
    "ehdgus5178@gmail.com",
    "",
    ""};

/* Basic constants and macros */
#define byte4 4             /* header/footer 한 칸 크기 */
#define byte8 8             /* double word 크기 */
#define size4096 (1 << 10) /* 처음/추가로 힙을 늘릴 기본 크기: 4096 bytes */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* 힙 기준 포인터 */
static char *heap_listp;
static char *rover;

/* size와 alloc bit를 한 워드에 합치기 */
#define content(size, alloc) ((size) | (alloc))

/* p가 가리키는 주소에서 4바이트 읽기/쓰기 */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* header/footer 값에서 size와 alloc bit 꺼내기 */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* bp는 payload 시작 주소라고 가정 */
#define headaddress(bp) ((char *)(bp) - byte4)
#define footeraddress(bp) ((char *)(bp) + GET_SIZE(headaddress(bp)) - byte8)

/* 다음 블록 / 이전 블록의 payload 시작 주소 계산 */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(headaddress(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - byte8)))

/* helper function prototypes */
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);


int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * byte4)) == (void *)-1)
        return -1;

    PUT(heap_listp, 0);                          
    PUT(heap_listp + byte4, content(byte8, 1));    
    PUT(heap_listp + 2 * byte4, content(byte8, 1));
    PUT(heap_listp + 3 * byte4, content(0, 1));    

    heap_listp += 2 * byte4;
    rover = heap_listp;

    if (extend_heap(size4096 / byte4) == NULL)
        return -1;

    return 0;
}
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words % 2) ? (words + 1) * byte4 : words * byte4;

    if ((bp = mem_sbrk(size)) == (void *)-1)
        return NULL;

    PUT(headaddress(bp), content(size, 0));         
    PUT(footeraddress(bp), content(size, 0));         
    PUT(headaddress(NEXT_BLKP(bp)), content(0, 1)); 

    return coalesce(bp);
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(footeraddress(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(headaddress(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(headaddress(bp));

    if (prev_alloc && next_alloc) {
    }

    else if (prev_alloc && !next_alloc) {
        size += GET_SIZE(headaddress(NEXT_BLKP(bp)));
        PUT(headaddress(bp), content(size, 0));
        PUT(footeraddress(bp), content(size, 0));
    }

    else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(headaddress(PREV_BLKP(bp)));
        PUT(headaddress(PREV_BLKP(bp)), content(size, 0));
        PUT(footeraddress(bp), content(size, 0));
        bp = PREV_BLKP(bp);
    }

    else {
        size += GET_SIZE(headaddress(PREV_BLKP(bp))) + GET_SIZE(headaddress(NEXT_BLKP(bp)));
        PUT(headaddress(PREV_BLKP(bp)), content(size, 0));
        PUT(footeraddress(NEXT_BLKP(bp)), content(size, 0));
        bp = PREV_BLKP(bp);
    }

    if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp)))
        rover = bp;

    return bp;
}

void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    if (size == 0)
        return NULL;

    if (size <= byte8)
        asize = 2 * byte8;
    else
        asize = byte8 * ((size + byte8 + (byte8 - 1)) / byte8);

    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, (1 << 12));
    if ((bp = extend_heap(extendsize / byte4)) == NULL)
        return NULL;
    coalesce(bp);
    place(bp, asize);
    return bp;
}

static void *find_fit(size_t asize)
{
    char *oldrover = rover;
    char *best_bp = NULL;
    size_t best_size = -1;
    int lookahead = -1;

    do {
        if (!GET_ALLOC(headaddress(rover)) && asize <= GET_SIZE(headaddress(rover))) {
            size_t current_size = GET_SIZE(headaddress(rover));

            if (current_size == asize) {
                return rover;
            }

            if (best_bp == NULL) {
                best_bp = rover;
                best_size = current_size;
                if (asize <= 48) lookahead = 15;
                else lookahead = 7;
                
            }
            else if (current_size < best_size) {
                best_bp = rover;
                best_size = current_size;
            }
        }

        if (best_bp != NULL) {
            if (lookahead == 0) {
                rover = best_bp;
                return best_bp;
            }
            lookahead--;
        }

        rover = NEXT_BLKP(rover);
        if (GET_SIZE(headaddress(rover)) == 0) {
            rover = heap_listp;
        }
    } while (rover != oldrover);

    if (best_bp != NULL) {
        rover = best_bp;
        return best_bp;
    }

    return NULL;
}

static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(headaddress(bp));

    if ((csize - asize) >= (2 * byte8)) {
        PUT(headaddress(bp), content(asize, 1));
        PUT(footeraddress(bp), content(asize, 1));

        bp = NEXT_BLKP(bp);
        PUT(headaddress(bp), content(csize - asize, 0));
        PUT(footeraddress(bp), content(csize - asize, 0));
        char *next = NEXT_BLKP(bp);
        if (!GET_ALLOC(headaddress(next))) {
            size_t merged_size = GET_SIZE(headaddress(bp)) + GET_SIZE(headaddress(next));
            PUT(headaddress(bp), content(merged_size, 0));
            PUT(footeraddress(next), content(merged_size, 0));
        }
    }
    else {
        char *next2 = NEXT_BLKP(bp);
        if (!GET_ALLOC(headaddress(next2))) {
            PUT(headaddress(bp), content(asize, 1));
            PUT(footeraddress(bp), content(asize, 1));

            bp = NEXT_BLKP(bp);
            PUT(headaddress(bp), content(csize - asize, 0));
            PUT(footeraddress(bp), content(csize - asize, 0));

            size_t merged_size2 = GET_SIZE(headaddress(bp)) + GET_SIZE(headaddress(NEXT_BLKP(bp)));
            PUT(headaddress(bp), content(merged_size2, 0));
            PUT(footeraddress(NEXT_BLKP(bp)), content(merged_size2, 0));
        }
        else{
            PUT(headaddress(bp), content(csize, 1));
            PUT(footeraddress(bp), content(csize, 1));
        }
    }
}

void mm_free(void *ptr)
{
    size_t size = GET_SIZE(headaddress(ptr));

    PUT(headaddress(ptr), content(size, 0));
    PUT(footeraddress(ptr), content(size, 0));
    coalesce(ptr);
}


void *mm_realloc(void *ptr, size_t size)
{
    size_t asize,oldsize,nextsize,prevsize,copySize,remain,next_alloc,prev_alloc;
    void *newptr,*nextbp,*prevbp;

    if (ptr == NULL)
        return mm_malloc(size);

    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    if (size <= byte8)
        asize = 2 * byte8;
    else
        asize = byte8 * ((size + byte8 + (byte8 - 1)) / byte8);

    oldsize = GET_SIZE(headaddress(ptr));

    if (oldsize >= asize)
        return ptr;

    nextbp = NEXT_BLKP(ptr);
    next_alloc = GET_ALLOC(headaddress(nextbp));
    nextsize = GET_SIZE(headaddress(nextbp));

    if (!next_alloc && (oldsize + nextsize) >= asize) {
        remain = oldsize + nextsize - asize;

        if (remain >= 2 * byte8) {
            PUT(headaddress(ptr), content(asize, 1));
            PUT(footeraddress(ptr), content(asize, 1));

            nextbp = NEXT_BLKP(ptr);
            PUT(headaddress(nextbp), content(remain, 0));
            PUT(footeraddress(nextbp), content(remain, 0));
            char *next = NEXT_BLKP(nextbp);
            if (!GET_ALLOC(headaddress(next))) {
                size_t merged_size = GET_SIZE(headaddress(nextbp)) + GET_SIZE(headaddress(next));
                PUT(headaddress(nextbp), content(merged_size, 0));
                PUT(footeraddress(next), content(merged_size, 0));
            }
        } else {
            char *next2 = NEXT_BLKP(ptr);
            if (!GET_ALLOC(headaddress(next2))) {
                PUT(headaddress(ptr), content(asize, 1));
                PUT(footeraddress(ptr), content(asize, 1));

                nextbp = NEXT_BLKP(ptr);
                PUT(headaddress(nextbp), content(oldsize + nextsize - asize, 0));
                PUT(footeraddress(nextbp), content(oldsize + nextsize - asize, 0));

                size_t merged_size2 = GET_SIZE(headaddress(nextbp)) + GET_SIZE(headaddress(NEXT_BLKP(nextbp)));
                PUT(headaddress(nextbp), content(merged_size2, 0));
                PUT(footeraddress(NEXT_BLKP(nextbp)), content(merged_size2, 0));
            }
            else{
                PUT(headaddress(ptr), content(oldsize + nextsize, 1));
                PUT(footeraddress(ptr), content(oldsize + nextsize, 1));
            }
        }
        return ptr;
    }
    prevbp = PREV_BLKP(ptr);
    prev_alloc = GET_ALLOC(headaddress(prevbp));
    prevsize = GET_SIZE(headaddress(prevbp));

    if (!prev_alloc && (oldsize + prevsize) >= asize) {
        remain = oldsize + prevsize - asize;

        memmove(prevbp, ptr, oldsize - byte8);

        if (remain >= 2 * byte8) {
            PUT(headaddress(prevbp), content(asize, 1));
            PUT(footeraddress(prevbp), content(asize, 1));

            nextbp = NEXT_BLKP(prevbp);
            PUT(headaddress(nextbp), content(remain, 0));
            PUT(footeraddress(nextbp), content(remain, 0));

        } else {
            PUT(headaddress(prevbp), content(oldsize + prevsize, 1));
            PUT(footeraddress(prevbp), content(oldsize + prevsize, 1));
        }

        return prevbp;
    }

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;

    copySize = oldsize - byte8;
    if (size < copySize)
        copySize = size;

    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    return newptr;
}