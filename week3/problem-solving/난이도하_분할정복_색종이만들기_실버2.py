# 분할정복 - 색종이 만들기 (백준 실버2)
# 문제 링크: https://www.acmicpc.net/problem/2630
n = int(input())
alist=[]
for i in range(n):

    alist.append(list(map(int,input().split())))

white=0
blue=0

def divide(left,leftup,size):
    global white,blue
    first=alist[left][leftup]
    for i in range(left,left+size):
        for j in range(leftup,leftup+size):
            if alist[i][j] != first:
                mid=size//2
                divide(left,leftup,mid)
                divide(left,leftup+mid,mid)
                divide(left+mid,leftup,mid)
                divide(left+mid,leftup+mid,mid)
                return
    if first==0:
        white+=1
        return
    else:
        blue+=1
        return

divide(0,0,n)
print(white)
print(blue)

    
    
    
        
            
    

