# 정수론 - 골드바흐의 추측 (백준 실버2)
# 문제 링크: https://www.acmicpc.net/problem/9020

T=int(input())
def is_prime(n):
    if n < 2:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0:
        return False
    for i in range(3, int(n**0.5) + 1, 2):
        if n % i == 0:
            return False
    return True
for i in range(T):
    n=int(input())
    n1=int(n/2)
    if is_prime(n1):
        print(n1,n1)
        continue
    for j in range(1,n1):
        if is_prime(n1-j) and is_prime(n1+j):
            print(n1-j,n1+j)
            break


