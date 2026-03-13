# 스택 - 스택 (백준 실버 4)
# 문제 링크: https://www.acmicpc.net/problem/10828
import sys
input = sys.stdin.readline
n=int(input())

astack=[]
bstack=[]

def push(num):
    astack.append(num)
def pop():
    if not astack:
        return -1
    return astack.pop()
def size():
    return len(astack)
def empty():
    if len(astack)==0:
        return 1
    else:
        return 0
def top():
    if len(astack)==0:
        return -1
    x=len(astack)
    return astack[x-1]


for i in range(n):
    string=input().split()
    if len(string)==2:
        x=string.pop()
    y=string.pop()
    if y=="push":
        push(int(x))
    if y=="pop":
        print(pop())
    if y=="size":
        print(size())
    if y=="empty":
        print(empty())
    if y=="top":
        print(top())



