from collections import deque
import sys

input = sys.stdin.readline

n = int(input())
k = int(input())

board = []
for i in range(n):
    row = []
    for j in range(n):
        row.append(0)
    board.append(row)

for i in range(k):
    r, c = map(int, input().split())
    board[r - 1][c - 1] = 1   # 사과

l = int(input())

turn_info = {}
for i in range(l):
    x, c = input().split()
    turn_info[int(x)] = c

# 오른쪽, 아래, 왼쪽, 위
dx = [0, 1, 0, -1]
dy = [1, 0, -1, 0]

direction = 0   # 처음엔 오른쪽

snake = deque()
snake.append((0, 0))

visited = []
for i in range(n):
    row = []
    for j in range(n):
        row.append(False)
    visited.append(row)

visited[0][0] = True

time = 0
head_x = 0
head_y = 0

while True:
    time += 1

    nx = head_x + dx[direction]
    ny = head_y + dy[direction]

    # 벽에 부딪히는 경우
    if nx < 0 or nx >= n or ny < 0 or ny >= n:
        break

    # 자기 몸에 부딪히는 경우
    if visited[nx][ny] == True:
        break

    # 머리 이동
    snake.append((nx, ny))
    visited[nx][ny] = True

    # 사과가 있으면 꼬리 유지
    if board[nx][ny] == 1:
        board[nx][ny] = 0
    else:
        # 사과가 없으면 꼬리 제거
        tail_x, tail_y = snake.popleft()
        visited[tail_x][tail_y] = False

    # 현재 머리 위치 갱신
    head_x = nx
    head_y = ny

    # 방향 전환
    if time in turn_info:
        if turn_info[time] == 'D':
            direction = (direction + 1) % 4
        else:   # 'L'
            direction = (direction - 1) % 4

print(time)