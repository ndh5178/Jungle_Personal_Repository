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
#define WSIZE 4             /* header/footer 한 칸 크기 */
#define DSIZE 8             /* double word 크기 */
#define CHUNKSIZE (1 << 12) /* 처음/추가로 힙을 늘릴 기본 크기: 4096 bytes */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* 힙 기준 포인터 */
static char *heap_listp;

/* size와 alloc bit를 한 워드에 합치기 */
#define PACK(size, alloc) ((size) | (alloc))

/* p가 가리키는 주소에서 4바이트 읽기/쓰기 */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* header/footer 값에서 size와 alloc bit 꺼내기 */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* bp는 payload 시작 주소라고 가정 */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* 다음 블록 / 이전 블록의 payload 시작 주소 계산 */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* helper function prototypes */
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);

/*
 * mm_init - malloc 패키지를 초기화합니다.
 */
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {
        return bp;
    }

    if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return bp;
    }

    if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return PREV_BLKP(bp);
    }

    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    return PREV_BLKP(bp);
}
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;

    PUT(heap_listp, 0);                          // alignment padding
    PUT(heap_listp + WSIZE, PACK(DSIZE, 1));    // prologue header
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));// prologue footer
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));    // epilogue header

    heap_listp += 2 * WSIZE;

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;

    return 0;
}
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

    if ((bp = mem_sbrk(size)) == (void *)-1)
        return NULL;

    PUT(HDRP(bp), PACK(size, 0));         
    PUT(FTRP(bp), PACK(size, 0));         
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); 

    return coalesce(bp);
}

/*
 * mm_malloc - brk 포인터를 증가시키는 방식으로 블록을 할당합니다.
 *     항상 정렬 단위의 배수 크기로 블록을 할당해야 합니다.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
        return NULL;
    else
    {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - 현재 이 단순 구현에서는 free를 호출해도 아무 일도 하지 않습니다.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - mm_malloc과 mm_free를 이용해 단순하게 구현되어 있습니다.
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
        copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
