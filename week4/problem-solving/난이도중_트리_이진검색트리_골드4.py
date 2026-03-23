# # 트리 - 이진 검색 트리 (백준 골드4)
# # 문제 링크: https://www.acmicpc.net/problem/5639
# import sys
# input=sys.stdin.readline
# sys.setrecursionlimit(10**6)

# class treenode:
#     def __init__(self, value):
#         self.value = value
#         self.left = None
#         self.right = None
#         self.prev = None

# nums = []
# while True:
#     try:
#         nums.append(int(input()))
#     except:
#         break
# leftnode=0
# rightnode=0
# root=treenode(nums[0])
# savenode=root
# root2=savenode
# i=1
# while i<len(nums):
#     if nums[i]<savenode.value:
#         leftnode=treenode(nums[i])
#         savenode.left=leftnode
#         leftnode.prev=savenode
#         savenode=savenode.left
#         i+=1
#     elif nums[i]>root2.value:
#         savenode=root2
#         rightnode=treenode(nums[i])
#         savenode.right=rightnode
#         rightnode.prev=savenode
#         savenode=savenode.right
#         root2=savenode
#         i+=1
#     elif nums[i]<root2.value and savenode.left ==None and nums[i] < savenode.prev.value:
#         rightnode=treenode(nums[i])
#         savenode.right=rightnode
#         rightnode.prev=savenode
#         savenode=savenode.right
#         i+=1
#     elif nums[i]<root2.value:
#         savenode=savenode.prev
#         rightnode=treenode(nums[i])
#         savenode.right=rightnode
#         rightnode.prev=savenode
#         i+=1
    
# def postorder(root):
#     """후위 순회: 왼쪽 → 오른쪽 → 루트"""
#     result = []
    
#     # TODO: root가 None이면 빈 리스트 반환
#     if root==None:
#         return []
#     pass
    
#     # TODO: 왼쪽 서브트리 순회
#     result=result+postorder(root.left)
#     pass
    
#     # TODO: 오른쪽 서브트리 순회
#     result=result+postorder(root.right)
#     pass
    
#     # TODO: 루트 값 추가
#     result.append(root.value)
#     pass
    
#     return result

# result=postorder(root)

# for i in result:
#     print(i)

import sys
input=sys.stdin.readline
sys.setrecursionlimit(10**6)
class treenode:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None
        self.prev = None

nums = []
while True:
    try:
        nums.append(int(input()))
    except:
        break

root=treenode(nums[0])
stack=[root]

for i in range(1, len(nums)):
    node = treenode(nums[i])

    if nums[i] < stack[-1].value:
        stack[-1].left = node
        stack.append(node)
    else:
        parent = None
        while stack and stack[-1].value < nums[i]:
            parent = stack.pop()
        parent.right = node
        stack.append(node)

def postorder(root):
    if root is None:
        return []
    
    result = []
    result += postorder(root.left)
    result += postorder(root.right)
    result.append(root.value)
    
    return result

result = postorder(root)

for i in result:
    print(i)
