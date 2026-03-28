# 그리디 - 신입 사원 (백준 실버1)
# 문제 링크: https://www.acmicpc.net/problem/1946

import sys
input=sys.stdin.readline

aum=int(input())
for _ in range(aum):
    rank=[]
    bum=int(input())
    for i in range(bum):
        rank.append(list(map(int,input().split())))
    if bum == 1:
        print(bum)
        continue
    if bum == 2:
        if [1,1] in rank:
            print(1)
            continue
        print(2)
        continue
    rank.sort()
    result=[]
    suru=rank[0]
    munjup=rank[1]
    for i in range(2,len(rank)):
        change=rank[i]
        if suru[0]>change[0]:
            save=change
            change=suru
            suru=save
        if munjup[1]>change[1]:
            save=change
            change=munjup
            munjup=save
        if suru[0]<change[0] and suru[1]<change[1]:
            continue
        if munjup[0]<change[0] and munjup[1]<change[1]:
            continue
        result.append(change)
    
    print(len(result)+2)
            
