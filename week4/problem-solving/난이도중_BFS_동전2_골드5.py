# BFS - 동전 2 (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/2294
import sys
input=sys.stdin.readline
from collections import deque

num,kum=list(map(int,input().split()))
coin=[]
queue=deque()
for i in range(num):
    coin.append(int(input()))
    queue.append((coin[i],1))
visited=set()
visited.add(coin[0])
while queue:
    value,count=queue.popleft()
    if value == kum:
        print(count)
        exit()
    for i in coin:
        savevalue=0
        savevalue=value+i
        if savevalue == kum:
            print(count+1)
            exit()
        if savevalue>kum:
            continue
        if savevalue not in visited:
            queue.append((savevalue,count+1))
            visited.add(savevalue)
    if savevalue == kum:
        break
if not queue:
    print(-1)
