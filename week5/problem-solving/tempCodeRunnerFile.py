# 그리디 - 멀티탭 스케줄링 (백준 골드1)
# 문제 링크: https://www.acmicpc.net/problem/1700
import sys
input = sys.stdin.readline

aum,bum=list(map(int,input().split()))
multitab={}
for i in range(bum):
    multitab[i+1]=0
cum=list(map(int,input().split()))
save=0
for i in cum:
    if save == i:
        continue
    multitab[i]+=1
    save=i
plug=[]
for i in cum:
    if save == i:
        continue
    plug.append([multitab[i],i])
    multitab[i]-=1
    save=i

first=[]
first2=[]
count=0
i=0
while i<aum:
    if cum[i] in first2:
        aum+=1
        i+=1
        continue
    first2.append(cum[i])
    first.append(plug[i])
    i+=1
# for i in range(aum):
#     if cum[i] in first2:
#         aum+=1
#         continue
#     first2.append(cum[i])
#     first.append(plug[i])
for i in range(aum,len(plug)):
    a,b=plug[i]
    for j in range(len(first)):
        a1,b1=first[j]
        if b == b1:
            first[j]=plug[i]
            break
    if b == b1:
        continue
    first.sort()
    first[0]=plug[i]
    count+=1
    
print(count)

