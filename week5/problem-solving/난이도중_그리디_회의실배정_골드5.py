# 그리디 - 회의실 배정 (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/1931
import sys
input = sys.stdin.readline

aum=int(input())

meeting=[]
for i in range(aum):
    meeting.append(tuple(map(int,input().split())))
meeting.sort(key=lambda x:(x[1],x[0]))
firstmeet=meeting[0][1]
count=1
for k in range(1,len(meeting)):
    i,j=meeting[k]
    if firstmeet<=i:
        firstmeet=j
        count+=1
print(count)
