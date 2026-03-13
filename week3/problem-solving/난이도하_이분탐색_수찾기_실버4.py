# 이분탐색 - 수 찾기 (백준 실버4)
# 문제 링크: https://www.acmicpc.net/problem/1920

n=int(input())

a=list(map(int,input().split()))

n2=int(input())

a2=list(map(int,input().split()))

a.sort()


def binary_search(arr, target):
    """
    이분 탐색 구현
    
    Args:
        arr: 정렬된 배열
        target: 찾을 값
    
    Returns:
        target의 인덱스 (없으면 -1)
    """

    left = 0
    right = len(arr) - 1
    
    # TODO: left가 right보다 작거나 같을 때까지 반복


    ## 중간 인덱스 계산
    ## arr[mid]와 target 비교
    ## 같으면 mid 반환
    ## target이 더 크면 left = mid + 1
    ## target이 더 작으면 right = mid - 1
    while left<=right:
        mid = (left + right) // 2
        if arr[mid]==target:
            return 1
        if target>arr[mid]:
            left = mid + 1
        if target<arr[mid]:
            right = mid -1
    pass
    
    return 0

for i in a2:
    print(binary_search(a,i))

    

