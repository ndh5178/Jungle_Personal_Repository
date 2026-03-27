graph = {

    'A': ['B', 'C'],

    'B': ['D', 'E'],

    'C': ['F'],

    'D': ['G'],

    'E': ['G'],

    'G': ['H'],

    'H': ['F'],

  }

def dfs(graph, start):

    visited = []

    back_edges = set()

    stack = [start]

    while stack:

        node = stack.pop()

        if node not in visited:
            if node not in graph:
                visited.append(node)
                continue
            visited.append(node)
            stack.extend(graph[node][::-1])
            for i in graph[node]:
                if i not in graph:
                    back_edges.add((node,i))
        else:
            continue
    return visited, sorted(list(back_edges))

x,y=dfs(graph,'A')
print(x,y)