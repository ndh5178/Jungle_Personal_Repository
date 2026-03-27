# 그리디 - 동전 0 (백준 실버4)
# 문제 링크: https://www.acmicpc.net/problem/11047

num,kum=list(map(int,input().split()))
coins=[]
for i in range(num):
    coins.append(int(input()))
coins=coins[::-1]
count=0
for i in coins:
    momey=kum//i
    if momey>0:
        count+=momey
        kum%=i
print(count)