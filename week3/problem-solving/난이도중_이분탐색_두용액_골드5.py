# 이분탐색 - 두 용액 (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/2470


N=int(input())
Nlist=list(map(int,input().split(" ")))

Nlist.sort()

left=0
right=len(Nlist)-1

result=2000000001
sumnum=0
savea=0
saveb=0

while left<right:
    sumnum=Nlist[left]+Nlist[right]
    if Nlist[left]+Nlist[right]==0:
        savea=Nlist[left]
        saveb=Nlist[right]
        break
    if sumnum<0:
        if abs(sumnum)<abs(result):
            result=sumnum
            savea=Nlist[left]
            saveb=Nlist[right]
        left+=1
    if sumnum>0:
        if abs(sumnum)<abs(result):
            result=sumnum
            savea=Nlist[left]
            saveb=Nlist[right]
        right-=1
        
print(savea,saveb)




