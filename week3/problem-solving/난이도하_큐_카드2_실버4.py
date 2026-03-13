# 큐 - 카드2 (백준 실버4)
# 문제 링크: https://www.acmicpc.net/problem/2164
from collections import deque
n=int(input())

if n==1 or n==2:
    print(n)
    exit()
    
queue1=[]
for i in range(n):
    queue1.append(i+1)
queue = deque(queue1)

while len(queue) != 1:
    queue.popleft()
    queue.append(queue.popleft())
print(queue[0])
