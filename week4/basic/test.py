from collections import deque

graph = {
        0: [1, 2],
        1: [0, 2],
        2: [0, 1, 3],
        3: [2]
    }

queue = deque()
li=[]
queue.extend(graph[0])
print(queue)
print(queue.popleft())