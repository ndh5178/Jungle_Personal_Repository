# 그리디 - 멀티탭 스케줄링 (백준 골드1)
# 문제 링크: https://www.acmicpc.net/problem/1700
# import sys
# input = sys.stdin.readline

# aum,bum=list(map(int,input().split()))
# multitab={}
# for i in range(bum):
#     multitab[i+1]=0
# cum=list(map(int,input().split()))
# save=0
# for i in cum:
#     if save == i:
#         continue
#     multitab[i]+=1
#     save=i
# plug=[]
# for i in cum:
#     if save == i:
#         continue
#     plug.append([multitab[i],i])
#     multitab[i]-=1
#     save=i

# first=[]
# first2=[]
# count=0
# i=0
# while i<aum:
#     if cum[i] in first2:
#         aum+=1
#         i+=1
#         continue
#     first2.append(cum[i])
#     first.append(plug[i])
#     i+=1
# for i in range(aum,len(plug)):
#     a,b=plug[i]
#     for j in range(len(first)):
#         a1,b1=first[j]
#         if b == b1:
#             first[j]=plug[i]
#             break
#     if b == b1:
#         continue
#     first.sort()
#     first[0]=plug[i]
#     count+=1
    
# print(count)


import sys
input = sys.stdin.readline

aum, bum = map(int, input().split())
cum = list(map(int, input().split()))

plug = []
count = 0

for i in range(bum):
    now = cum[i]

    if now in plug:
        continue

    if len(plug) < aum:
        plug.append(now)
        continue

    remove_index = -1
    farthest = -1

    for j in range(len(plug)):
        device = plug[j]

        next_use = -1
        for k in range(i + 1, bum):
            if cum[k] == device:
                next_use = k
                break

        if next_use == -1:
            remove_index = j
            break

        if next_use > farthest:
            farthest = next_use
            remove_index = j

    plug[remove_index] = now
    count += 1

print(count)