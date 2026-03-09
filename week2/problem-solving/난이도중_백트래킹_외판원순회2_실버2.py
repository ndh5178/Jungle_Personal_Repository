import random

n = int(input())

city = []
for i in range(n):
    row = []
    for j in range(n):
        row.append(random.randrange(1, 10))
    city.append(row)

for i in range(n):
    city[i][i] = 0

answer = 1000000000

def backtrack(start, current_city, visited_city, cost, count):
    global answer

    if count == n and city[current_city][start] != 0:
        total_cost = cost + city[current_city][start]
        if total_cost < answer:
            answer = total_cost
            return

    for next_city in range(n):
        if visited_city[next_city] == False and city[current_city][next_city] != 0:
            visited_city[next_city] = True
            backtrack(start, next_city, visited_city, cost + city[current_city][next_city], count + 1)
            visited_city[next_city] = False

for i in range(n):
    print(city[i])

visited_city = [False] * n
visited_city[0] = True

backtrack(0, 0, visited_city, 0, 1)
print("최소 비용:", answer)