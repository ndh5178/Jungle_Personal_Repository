import sys
from collections import deque
input = sys.stdin.readline



def L():
    if len(lstack) != 0:
        rstack.append(lstack.pop())
    return

def D():
    if len(rstack) != 0:
        lstack.append(rstack.pop())
    return

def B():
    if len(lstack) != 0:
        lstack.pop()
    return
def P(s):
    lstack.append(s)

s=input().rstrip()
lstack=s[:]
lstack=deque(lstack)
rstack=[]
rstack=deque(rstack)

n=int(input())

for i in range(n):
    string=input().split()

    if string[0]=="L":
        L()
    if string[0]=="D":
        D()
    if string[0]=="B":
        B()
    if string[0]=="P":
        P(string[1])


lstack.extend(reversed(rstack))
result = ''.join(lstack)
print(result)

