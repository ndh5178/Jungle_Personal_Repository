# 파이썬 문법 - 최댓값 (백준 브론즈3)
# 문제 링크: https://www.acmicpc.net/problem/2562
numlist=[]
for i in range(0,9):
    inum=int(input())
    numlist.append(inum)

maxnum=max(numlist)

indnum=numlist.index(maxnum)
print(maxnum)
print(indnum+1)



    





