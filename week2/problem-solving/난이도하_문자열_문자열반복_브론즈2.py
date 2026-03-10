# 문자열 - 문자열 반복 (백준 브론즈2)
# 문제 링크: https://www.acmicpc.net/problem/2675

ipnum=int(input())
r=""
s=""
for i in range(ipnum):
    r,s=input().split(" ")
    r=int(r)

    for j in s:
        print(j*r,end="")
    print()
    r=""
    s=""




