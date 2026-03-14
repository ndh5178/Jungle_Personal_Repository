# 해시 테이블 - 세 수의 합 (백준 골드4)
# 문제 링크: https://www.acmicpc.net/problem/2295
import sys
input = sys.stdin.readline

n=int(input())

linum=[]
for i in range(n):
    linum.append(int(input()))
linum=sorted(linum)
stlinum=sorted(linum, reverse=True)

sumli = set()
for i in range(n):
    for j in range(i,n):
        sumli.add(linum[i] + linum[j])

for d in stlinum:
    for c in linum:
        if d-c in sumli:
            print(d)
            exit()




