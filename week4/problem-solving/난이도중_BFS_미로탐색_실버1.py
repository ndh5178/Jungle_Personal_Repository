# BFS - 미로 탐색 (백준 실버1)
# 문제 링크: https://www.acmicpc.net/problem/2178

import sys

input = sys.stdin.readline
from collections import deque

num, mum = list(map(int, input().split()))
game = []
visited = []
for i in range(num):
    game.append([])
    visited.append([])
for i in range(num):
    miro = input().strip()
    for j in miro:
        game[i].append(j)
        visited[i].append(False)


queue = deque()
queue.append((0, 0, 0))
visited[0][0] = True

while queue:
    x, y, count = queue.popleft()
    if x == num - 1 and y == mum - 1:
        print(count+1)
        break
    if y + 1 < mum and game[x][y + 1] == "1" and visited[x][y + 1] == False:
        queue.append((x, y + 1, count + 1))
        visited[x][y + 1] = True
    if x + 1 < num and game[x + 1][y] == "1" and visited[x + 1][y] == False:
        queue.append((x + 1, y, count + 1))
        visited[x + 1][y] = True
    if y - 1 > -1 and game[x][y - 1] == "1" and visited[x][y - 1] == False:
        queue.append((x, y - 1, count + 1))
        visited[x][y - 1] = True
    if x - 1 > -1 and game[x - 1][y] == "1" and visited[x - 1][y] == False:
        queue.append((x - 1, y, count + 1))
        visited[x - 1][y] = True
