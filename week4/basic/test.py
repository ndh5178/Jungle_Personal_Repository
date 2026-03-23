# 후위 순회 - 왼쪽 -> 오른쪽 -> 루트
# 전위 순회 - 루트 -> 왼쪽 -> 오른쪽
# 전위로 들어올 input = 50 30 24 5 28 45 98 52 60
import sys
sys.setrecursionlimit(10**6)
preorder = []

# EOF까지 끝까지 읽어서 preorder에 넣는다. ctrl + D를 눌러서 EOF 표현
for line in sys.stdin:
    preorder.append(int(line.strip()))

# print(preorder)
# 큰 수가 나오면 일단 오른쪽 노드, 작은 수가 나오면 왼쪽 노드 즉, root 가 50일때 , 98부터 오른쪽 노드

def postorder(start, end):
    if start > end:
        return

    root = preorder[start]

    mid = end + 1
    for i in range(start+1, end+1):
        if preorder[i] > root:
            mid = i
            break

    postorder(start+1, mid-1)
    postorder(mid, end)

    print(root)

postorder(0, len(preorder) - 1)