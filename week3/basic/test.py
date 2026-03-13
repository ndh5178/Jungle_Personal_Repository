def merge(arr, left, mid, right):
    """
    두 개의 정렬된 부분 배열을 병합하는 함수
    
    Args:
        arr: 원본 배열
        left: 왼쪽 부분의 시작 인덱스
        mid: 왼쪽 부분의 끝 인덱스
        right: 오른쪽 부분의 끝 인덱스
    """
    # TODO: 왼쪽과 오른쪽 부분 배열을 임시 배열로 복사
    left_arr=[]
    right_arr=[]
    result=[]
    for i in range(left,mid+1):
        left_arr.append(arr[i])
    for i in range(mid+1,right+1):
        right_arr.append(arr[i])

    pass
    
    # TODO: 두 배열을 병합
    pass
    
    
    # TODO: left_arr와 right_arr를 비교하며 작은 값을 arr에 복사
    leftpointer=0
    rightpointer=0
    while leftpointer!=len(left_arr) and rightpointer!=len(right_arr):
        if left_arr[leftpointer]<=right_arr[rightpointer]:
            result.append(left_arr[leftpointer])
            leftpointer+=1
        else:
            result.append(right_arr[rightpointer])
            rightpointer+=1
    pass
    
    # TODO: 남은 원소들을 복사
    # left_arr에 남은 원소가 있으면 복사
    # right_arr에 남은 원소가 있으면 복사
    if leftpointer==len(left_arr):
        while rightpointer!=len(right_arr):
            result.append(right_arr[rightpointer])
            rightpointer+=1
    else:
        while leftpointer!=len(left_arr):
            result.append(left_arr[leftpointer])
            leftpointer+=1
    for i in range(len(result)):
        arr[i]=result[i]
    pass

arr=[2, 5, 8, 1, 3, 7]
print(arr)
print(merge(arr,0,2,5))
print(arr)
