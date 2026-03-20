# 트리 - 트리 만들기 (백준 실버4)
# 문제 링크: https://www.acmicpc.net/problem/14244
num,mum=list(map(int,input().split()))
mum2=1
result=set()

for i in range(num-1):
    if i==0:
        print(i,i+1)
        continue
    if mum2<mum:
        print(1,mum2+1)
        mum2+=1
        continue
    print(i,i+1)



