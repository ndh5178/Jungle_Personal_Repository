# 배열 - 평균은 넘겠지 (백준 브론즈1)
# 문제 링크: https://www.acmicpc.net/problem/4344

ipnum=int(input())

stdnum=[]

sumnum=0
abnum=0
abupstd=0
for i in range(ipnum):
    stdnum.append(list(map(int,input().split(" "))))

for i in range(ipnum):
    for j in range(len(stdnum[i])):
        sumnum=sum(stdnum[i])-stdnum[i][0]
        
for i in range(ipnum):
    sumnum=sum(stdnum[i])-stdnum[i][0]
    abnum=sumnum/stdnum[i][0]
    for j in range(1,len(stdnum[i])):
        if stdnum[i][j]>abnum:
            abupstd+=1
    print(str(round((abupstd/stdnum[i][0])*100,3))+"%")
    abupstd=0


