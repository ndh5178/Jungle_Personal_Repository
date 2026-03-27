# 그리디 - 잃어버린 괄호 (백준 실버2)
# 문제 링크: https://www.acmicpc.net/problem/1541
import sys
input=sys.stdin.readline

num=list(map(str,input().split("-")))
result=0

selected=0
if '+' in num[0]:
    num[0]=num[0].split('+')
    for i in num[0]:
        result+=int(i)
else:
    result+=int(num[0])
selected=result

for i in range(1,len(num)):
    result=0
    if '+' in num[i]:
        num[i]=num[i].split('+')
        for j in num[i]:
            result+=int(j)
    else:
        result+=int(num[i])
    selected-=result
print(selected)