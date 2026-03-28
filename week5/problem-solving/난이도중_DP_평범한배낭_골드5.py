# DP - 평범한 배낭 (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/12865
import sys
input=sys.stdin.readline

num,kum=list(map(int,input().split()))
bag=[]
for i in range(num):
    wum,vum=list(map(int,input().split()))
    bag.append((wum,vum))
bag.sort(key=lambda x:-x[0])

dp=[]
for i in range(kum + 1):
    dp.append(0)

for i in range(num):
    weight, value = bag[i]

    for j in range(kum, weight - 1, -1):
        if dp[j] < dp[j - weight] + value:
            dp[j] = dp[j - weight] + value

print(dp[kum])