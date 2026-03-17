# 큐 - 가운데를 말해요 (백준 골드2)
# 문제 링크: https://www.acmicpc.net/problem/1655
import sys
input = sys.stdin.readline
from heapq import heappush, heappop

n = int(input())

min_heap = []
max_heap = []

for i in range(n):
    m = int(input())
    if len(min_heap)==len(max_heap):
        heappush(max_heap,-m)
    else:
        len(min_heap)!=len(max_heap)
        heappush(min_heap,m)
    if i==0:
        print(-1*max_heap[0])
        continue
    if min_heap[0]<(-1*max_heap[0]):
        x=heappop(min_heap)
        y=heappop(max_heap)
        heappush(max_heap,-x)
        heappush(min_heap,-y)
    
    print(-1*max_heap[0])
    

    



