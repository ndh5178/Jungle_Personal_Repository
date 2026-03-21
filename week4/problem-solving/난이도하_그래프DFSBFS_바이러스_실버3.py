# 그래프, DFS, BFS - 바이러스 (백준 실버3)
# 문제 링크: https://www.acmicpc.net/problem/2606
import sys
from collections import deque
input = sys.stdin.readline

num=int(input())
graph={}
for i in range(num):
    graph[i+1]=[]
mum=int(input())

for i in range(mum):
    curcump,nextcum=list(map(int,input().split()))
    graph[curcump].append(nextcum)
    graph[nextcum].append(curcump)

visited=deque()
visitednum=1
result=set()
result.add(visitednum)
visited.append(visitednum)
while visited:
    visitednum=visited.popleft()
    for i in graph[visitednum]:
        if i not in result:
            visited.append(i)
            result.add(i)

print(len(result)-1)
