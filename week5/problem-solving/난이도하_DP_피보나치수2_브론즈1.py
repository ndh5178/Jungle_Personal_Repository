# DP - 피보나치 수 2 (백준 브론즈 1)
# 문제 링크: https://www.acmicpc.net/problem/2748

import sys
input=sys.stdin.readline

num=int(input())

def fib(n,memo=None):
    if memo is None:
        memo={}
    if n==0:
        return 0
    if n==1:
        return 1
    if n in memo:
        return memo[n]
    
    memo[n]=fib(n-1,memo)+fib(n-2,memo)
    return memo[n]

print(fib(num))