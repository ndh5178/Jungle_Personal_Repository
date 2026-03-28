# DP - 동전 (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/9084
import sys
input=sys.stdin.readline

tum=int(input())

for i in range(tum):
    num=int(input())
    coins=list(map(int,input().split()))
    money=int(input())
    dp=[]
    for j in range(money + 1):
        dp.append(0)
    dp[0]=1

    for coin in coins:
        for j in range(coin, money + 1):
            dp[j] += dp[j - coin]
    
    print(dp[money])




