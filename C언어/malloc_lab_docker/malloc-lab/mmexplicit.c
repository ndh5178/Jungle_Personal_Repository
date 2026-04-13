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



/* 워드(4바이트) 또는 더블 워드(8바이트) 정렬 */
#define ALIGNMENT 8

/* size를 ALIGNMENT의 배수로 올림 */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define byte4 4             /* header/footer 한 칸 크기 */
#define byte8 8             /* double word 크기 */
#define size4096 (1 << 10) /* 처음/추가로 힙을 늘릴 기본 크기: 4096 bytes */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* 힙 기준 포인터 */
static char *heap_listp;
static char *free_listp = NULL;

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

#define prevfree(bp) (*(char **)(bp))
#define nextfree(bp) (*(char **)((char *)(bp) + sizeof(void *)))
#define contentprev(bp, ptr) (prevfree(bp) = (char *)(ptr))
#define contentnext(bp, ptr) (nextfree(bp) = (char *)(ptr))
#define MIN_FREE_BLOCK_SIZE (byte4 + sizeof(void *) + sizeof(void *) + byte4)
/* helper function prototypes */
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);

/*
 * mm_init - malloc 패키지를 초기화합니다.
 */



int mm_init(void)
{
    free_listp=NULL;

    if ((heap_listp = mem_sbrk(4 * byte4)) == (void *)-1)
        return -1;

    PUT(heap_listp, 0);                          // alignment padding
    PUT(heap_listp + byte4, content(byte8, 1));    // prologue header
    PUT(heap_listp + 2 * byte4, content(byte8, 1));// prologue footer
    PUT(heap_listp + 3 * byte4, content(0, 1));    // epilogue header

    heap_listp += 2 * byte4;

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
    contentprev(bp,NULL);
    contentnext(bp,NULL);
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
        /* 여기에 추가해야합니다
         * 1. bp의 pred를 NULL로 만듭니다
         * 2. bp의 succ를 현재 free_listp로 연결합니다
         * 3. free_listp가 NULL이 아니면 기존 head의 pred를 bp로 바꿉니다
         * 4. free_listp를 bp로 갱신합니다
         */
        contentprev(bp,NULL);
        contentnext(bp,free_listp);
        if(free_listp!=NULL){
            contentprev(free_listp, bp);
        }
        free_listp=bp;
        return bp;
    }

    if (prev_alloc && !next_alloc) {
        /* 여기에 추가해야합니다
         * 1. NEXT_BLKP(bp)를 free list에서 제거합니다
         * 2. bp와 next 블록을 합친 크기로 header/footer를 갱신합니다
         * 3. 합쳐진 bp를 free_listp 앞에 다시 삽입합니다
         */
        char *next = NEXT_BLKP(bp);
        if (prevfree(next) != NULL) {
            contentnext(prevfree(next), nextfree(next));
        } else {
            free_listp = nextfree(next);
        }

        if (nextfree(next) != NULL) {
            contentprev(nextfree(next), prevfree(next));
        }
        size += GET_SIZE(headaddress(NEXT_BLKP(bp)));
        PUT(headaddress(bp), content(size, 0));
        PUT(footeraddress(bp), content(size, 0));
        contentprev(bp, NULL);
        contentnext(bp, free_listp);
        if (free_listp != NULL) {
            contentprev(free_listp, bp);
        }
        free_listp = bp;
        return bp;
    }

    if (!prev_alloc && next_alloc) {
        /* 여기에 추가해야합니다
         * 1. PREV_BLKP(bp)를 free list에서 제거합니다
         * 2. prev와 bp를 합친 크기로 header/footer를 갱신합니다
         * 3. 최종 블록 PREV_BLKP(bp)를 free_listp 앞에 다시 삽입합니다
         */
        char *prev = PREV_BLKP(bp);
        if (prevfree(prev) != NULL) {
            contentnext(prevfree(prev), nextfree(prev));
        } else {
            free_listp = nextfree(prev);
        }

        if (nextfree(prev) != NULL) {
            contentprev(nextfree(prev), prevfree(prev));
        }
        size += GET_SIZE(headaddress(PREV_BLKP(bp)));
        PUT(headaddress(PREV_BLKP(bp)), content(size, 0));
        PUT(footeraddress(bp), content(size, 0));
        contentprev(prev, NULL);
        contentnext(prev, free_listp);
        if (free_listp != NULL) {
            contentprev(free_listp, prev);
        }
        free_listp = prev;
        return PREV_BLKP(bp);
    }
    /* 여기에 추가해야합니다
     * 1. PREV_BLKP(bp)를 free list에서 제거합니다
     * 2. NEXT_BLKP(bp)를 free list에서 제거합니다
     * 3. prev + bp + next를 하나의 free 블록으로 병합합니다
     * 4. 최종 블록 PREV_BLKP(bp)를 free_listp 앞에 다시 삽입합니다
     */
    char *prev = PREV_BLKP(bp);
    char *next = NEXT_BLKP(bp);
    if (prevfree(prev) != NULL) {
    contentnext(prevfree(prev), nextfree(prev));
    } else {
        free_listp = nextfree(prev);
    }

    if (nextfree(prev) != NULL) {
        contentprev(nextfree(prev), prevfree(prev));
    }
    if (prevfree(next) != NULL) {
    contentnext(prevfree(next), nextfree(next));
    } else {
        free_listp = nextfree(next);
    }

    if (nextfree(next) != NULL) {
        contentprev(nextfree(next), prevfree(next));
    }
    size += GET_SIZE(headaddress(PREV_BLKP(bp))) + GET_SIZE(headaddress(NEXT_BLKP(bp)));
    PUT(headaddress(PREV_BLKP(bp)), content(size, 0));
    PUT(footeraddress(NEXT_BLKP(bp)), content(size, 0));
    contentprev(prev, NULL);
    contentnext(prev, free_listp);
    if (free_listp != NULL) {
        contentprev(free_listp, prev);
    }
    free_listp = prev;
    return PREV_BLKP(bp);
}
/*
 * mm_free - 현재 이 단순 구현에서는 free를 호출해도 아무 일도 하지 않습니다.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(headaddress(ptr));

    PUT(headaddress(ptr), content(size, 0));
    PUT(footeraddress(ptr), content(size, 0));
    coalesce(ptr);
}

static void *find_fit(size_t asize)
{
    void *bp;

    /* 여기에 추가해야합니다
     * 1. heap_listp부터 도는 기존 implicit 순회를 지웁니다
     * 2. free_listp부터 시작합니다
     * 3. bp = nextfree(bp)로 이동하면서 맞는 free 블록을 찾습니다
     */
    for (bp = free_listp; bp != NULL; bp = nextfree(bp)) {
        if (asize <= GET_SIZE(headaddress(bp))) {
            return bp;
        }
    }

    return NULL;
}

static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(headaddress(bp));

    /* 여기에 추가해야합니다
     * 1. 현재 bp를 free list에서 제거합니다
     * 2. bp 앞뒤 free 노드가 있으면 서로 다시 연결합니다
     * 3. bp가 free_listp였다면 head를 갱신합니다
     */

    if (prevfree(bp) != NULL) {
        contentnext(prevfree(bp), nextfree(bp));
    } else {
        free_listp = nextfree(bp);
    }

    if (nextfree(bp) != NULL) {
        contentprev(nextfree(bp), prevfree(bp));
    }
    if ((csize - asize) >= MIN_FREE_BLOCK_SIZE) {
        PUT(headaddress(bp), content(asize, 1));
        PUT(footeraddress(bp), content(asize, 1));

        bp = NEXT_BLKP(bp);
        /* 여기에 추가해야합니다
         * 1. split 후 남은 free 블록의 pred를 NULL로 둡니다
         * 2. 남은 free 블록의 succ를 현재 free_listp로 연결합니다
         * 3. free_listp가 있으면 그 블록의 pred를 새 free 블록으로 바꿉니다
         * 4. free_listp를 새 free 블록으로 갱신합니다
         */
        PUT(headaddress(bp), content(csize - asize, 0));
        PUT(footeraddress(bp), content(csize - asize, 0));

        contentprev(bp, NULL);
        contentnext(bp, free_listp);
        if (free_listp != NULL) {
            contentprev(free_listp, bp);
        }
        free_listp = bp;
    }
    else {
        /* 여기는 추가로 free 블록을 넣지 않습니다
         * 위에서 remove만 정상적으로 끝나면 됩니다
         */
        PUT(headaddress(bp), content(csize, 1));
        PUT(footeraddress(bp), content(csize, 1));
    }
}

/*
 * mm_malloc - brk 포인터를 증가시키는 방식으로 블록을 할당합니다.
 *     항상 정렬 단위의 배수 크기로 블록을 할당해야 합니다.
 */
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

    if (asize < MIN_FREE_BLOCK_SIZE) asize = MIN_FREE_BLOCK_SIZE;

    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, size4096);
    if ((bp = extend_heap(extendsize / byte4)) == NULL)
        return NULL;

    place(bp, asize);
    return bp;
}



/*
 * mm_realloc - mm_malloc과 mm_free를 이용해 단순하게 구현되어 있습니다.
 */
// void *mm_realloc(void *ptr, size_t size)
// {
//     size_t asize;
//     size_t oldsize;
//     size_t nextsize;
//     size_t copySize;
//     size_t remain;
//     size_t next_alloc;
//     void *newptr;
//     void *nextbp;

//     if (ptr == NULL)
//         return mm_malloc(size);

//     if (size == 0) {
//         mm_free(ptr);
//         return NULL;
//     }

//     if (size <= byte8)
//         asize = 2 * byte8;
//     else
//         asize = byte8 * ((size + byte8 + (byte8 - 1)) / byte8);

//     oldsize = GET_SIZE(headaddress(ptr));

//     if (oldsize >= asize)
//         return ptr;

//     nextbp = NEXT_BLKP(ptr);
//     next_alloc = GET_ALLOC(headaddress(nextbp));
//     nextsize = GET_SIZE(headaddress(nextbp));

//     if (!next_alloc && (oldsize + nextsize) >= asize) {
//         remain = oldsize + nextsize - asize;

//         if (remain >= 2 * byte8) {
//             PUT(headaddress(ptr), content(asize, 1));
//             PUT(footeraddress(ptr), content(asize, 1));

//             nextbp = NEXT_BLKP(ptr);
//             PUT(headaddress(nextbp), content(remain, 0));
//             PUT(footeraddress(nextbp), content(remain, 0));
//         } else {
//             PUT(headaddress(ptr), content(oldsize + nextsize, 1));
//             PUT(footeraddress(ptr), content(oldsize + nextsize, 1));
//         }

//         return ptr;
//     }

//     newptr = mm_malloc(size);
//     if (newptr == NULL)
//         return NULL;

//     copySize = oldsize - byte8;
//     if (size < copySize)
//         copySize = size;

//     memcpy(newptr, ptr, copySize);
//     mm_free(ptr);
//     return newptr;
// }

void *mm_realloc(void *ptr, size_t size)
{
    void *newptr;
    size_t copySize;

    if (ptr == NULL)
        return mm_malloc(size);

    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;

    copySize = GET_SIZE(headaddress(ptr)) - byte8;
    if (size < copySize)
        copySize = size;

    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    return newptr;
}
