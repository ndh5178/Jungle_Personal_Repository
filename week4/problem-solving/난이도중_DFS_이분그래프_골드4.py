# DFS - 이분 그래프 (백준 골드4)
# 문제 링크: https://www.acmicpc.net/problem/1707
import sys
input=sys.stdin.readline
from collections import deque

kum=int(input())
for _ in range(kum):
    graph={}
    vum,eum=list(map(int,input().split()))
    for i in range(vum):
        graph[i+1]=[]
    for i in range(eum):
        a,b=list(map(int,input().split()))
        graph[a].append(b)
        graph[b].append(a)

    color = [-1] * (vum + 1)
    result = "YES"
    for start in range(1, vum+1):
        if color[start] != -1:
            continue

        queue = deque()
        queue.append(start)
        color[start] = 0

        while queue:
            vertices = queue.popleft()
            for i in graph[vertices]:
                if color[i] == -1:
                    color[i] = 1 - color[vertices]
                    queue.append(i)
                elif color[i] == color[vertices]:
                    result = "NO"
                    break
            if result == "NO":
                break
        if result == "NO":
            break

    print(result)