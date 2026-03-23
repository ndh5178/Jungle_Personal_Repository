# 그래프, DFS, BFS - DFS와 BFS (백준 실버2)
# 문제 링크: https://www.acmicpc.net/problem/1260
import sys
from collections import deque
input=sys.stdin.readline

num,mum,vum=list(map(int,input().split()))

graph={}

for i in range(num):
    graph[i+1]=[]

for i in range(mum):
    a,b=list(map(int,input().split()))
    graph[a].append(b)
    graph[b].append(a)

for i in graph:
    graph[i].sort()

def dfs(graph,start,dfsresult=None):
    if dfsresult==None:
        dfsresult=[]
    dfsresult.append(start)

    for i in graph[start]:
        if i not in dfsresult:
            dfs(graph,i,dfsresult)
    return dfsresult

queue=deque()
queue.append(vum)
bfsresult=[]
bfsresult.append(vum)
while queue:
    x=queue.popleft()
    for i in graph[x]:
        if i not in bfsresult:
            queue.append(i)
            bfsresult.append(i)

print(' '.join(map(str,dfs(graph,vum))))
print(' '.join(map(str,bfsresult)))

