# 트리 - 트리의 부모 찾기 (백준 실버2)
# 문제 링크: https://www.acmicpc.net/problem/11725

import sys
from collections import deque
input=sys.stdin.readline

num=int(input())
graph={}
for i in range(num):
    graph[i+1]=[]
for i in range(num-1):
    kum,vum=list(map(int,input().split()))
    graph[kum].append(vum)
    graph[vum].append(kum)


root=deque()
root.append(1)
result={}
result[1]=1

while root:
    x=root.popleft()
    for i in graph[x]:
        if i not in result:
            root.append(i)
            result[i]=x
print(result) 

        
for i in range(2,len(result)+1):
    print(result[i])
