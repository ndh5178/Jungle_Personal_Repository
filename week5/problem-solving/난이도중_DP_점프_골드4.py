# DP - 점프 (백준 골드4)
# 문제 링크: https://www.acmicpc.net/problem/2253
import sys
input=sys.stdin.readline

num,kum=list(map(int,input().split()))

game=[]
for i in range(num + 1):
    game.append(0)

for i in range(kum):
    mum=int(input())
    if mum == 2:
        print(-1)
        exit()
    game[mum]=10000000000

game[1]=1
game[2]=1
save=2
for i in range(3,num + 1):
    if game[i] == 10000000000:
        continue
    min=game[i-1]
    for j in range(save):
        if min>game[i-j-1]:
            min=game[i-j-1]
    if min == 10000000000:
        print(-1)
        exit()
    save=min+1
    game[i]=save
    if i + save > num:
        print(save+1)
        exit()
    if i + save == num:
        print(save)
        exit()

print(game[num])


# import sys
# input = sys.stdin.readline

# num, kum = map(int, input().split())

# blocked = []
# for i in range(num + 1):
#     blocked.append(False)

# for i in range(kum):
#     mum = int(input())
#     blocked[mum] = True

# if num == 1:
#     print(0)
#     sys.exit()

# if blocked[2]:
#     print(-1)
#     sys.exit()

# limit = 0
# while limit * (limit + 1) // 2 <= num:
#     limit += 1

# INF = 1000000000

# dp = []
# for i in range(num + 1):
#     row = []
#     for j in range(limit + 2):
#         row.append(INF)
#     dp.append(row)

# dp[1][0] = 0

# for i in range(2, num + 1):
#     if blocked[i]:
#         continue

#     for jump in range(1, limit + 1):
#         prev = i - jump
#         if prev < 1:
#             break

#         best = INF

#         if jump - 1 >= 0:
#             if dp[prev][jump - 1] < best:
#                 best = dp[prev][jump - 1]

#         if dp[prev][jump] < best:
#             best = dp[prev][jump]

#         if jump + 1 <= limit:
#             if dp[prev][jump + 1] < best:
#                 best = dp[prev][jump + 1]

#         if best != INF:
#             dp[i][jump] = best + 1

# answer = INF
# for jump in range(limit + 1):
#     if dp[num][jump] < answer:
#         answer = dp[num][jump]

# if answer == INF:
#     print(-1)
# else:
    # print(answer)