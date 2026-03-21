import sys
input = sys.stdin.readline

num = int(input())
board = []
for i in range(num):
    board.append(list(map(int,input().split())))

visited = [[False] * num for i in range(num)]

def dfs(x, y):
    # 범위 밖이면 실패
    if x < 0 or x >= num or y < 0 or y >= num:
        return False

    # 도착 지점이면 성공
    if board[x][y] == -1:
        return True

    # 이미 방문한 곳이면 다시 안 감
    if visited[x][y]:
        return False

    visited[x][y] = True

    jump = board[x][y]

    # 아래 or 오른쪽으로 이동
    return dfs(x + jump, y) or dfs(x, y + jump)


# 실행
if dfs(0, 0):
    print("HaruHaru")
else:
    print("Hing")