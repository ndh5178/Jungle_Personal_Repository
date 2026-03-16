# 분할정복 - 곱셈 (백준 실버1)
# 문제 링크: https://www.acmicpc.net/problem/1629

import sys

input = sys.stdin.readline

a, b, c = list(map(int, input().split()))


def xx(a, b, c):
    if b == 1:
        return a % c

    mid = xx(a, b // 2, c)

    if b % 2 == 0:
        return (mid * mid) % c
    if b % 2 != 0:
        return (mid * mid * a) % c


print(xx(a, b, c))
