# # 그래프, DFS, BFS - 연결 요소의 개수 (백준 실버2)
# # 문제 링크: https://www.acmicpc.net/problem/11724
# import sys
# from collections import deque
# input=sys.stdin.readline

# num,mum=list(map(int,input().split()))

# graph={}

# for i in range(num):
#     graph[i+1]=[]
# for i in range(mum):
#     a,b=list(map(int,input().split()))
#     graph[a].append(b)
#     graph[b].append(a)

# queue=deque()
# queue.append(1)
# bfsresult=[]
# bfsresult.append(1)
# k=1
# count=0
# while any(graph.values()):
#     if graph[k]==[]:
#         k+=1
#         continue
#     queue.append(k)
#     while queue:
#         x=queue.popleft()
#         y=len(graph[x])
#         for i in range(y):
#             s=graph[x].pop()
#             if s not in bfsresult:
#                 queue.append(s)
#                 bfsresult.append(s)
#     count+=1

# print(count)


import sys
from collections import deque
input = sys.stdin.readline

num, mum = map(int, input().split())

graph = {}
for i in range(num):
    graph[i + 1] = []

for i in range(mum):
    a, b = map(int, input().split())
    graph[a].append(b)
    graph[b].append(a)

visited = [False] * (num + 1)
count = 0

for k in range(1, num + 1):
    if visited[k] == True:
        continue

    queue = deque()
    queue.append(k)
    visited[k] = True
    count += 1

    while queue:
        x = queue.popleft()
        for i in graph[x]:
            if visited[i] == False:
                visited[i] = True
                queue.append(i)

print(count)