# 정수론 - 소수 찾기 (백준 브론즈2)
# 문제 링크: https://www.acmicpc.net/problem/1978

inpnum = int(input())
numlist = list(map(int, input().split(" ")))
count = 0
for i in numlist:
    def is_prime(i):
        if i == 1:
            return 0
        if i == 2 or i == 3 or i == 5 or i == 7:
            return 1

        if i % 2 == 0:
            return 0

        for j in range(3, int(i**0.5) + 1, 2):
            if i % j == 0:
                return 0
        return 1
    
    count+=is_prime(i)
        
print(count)
