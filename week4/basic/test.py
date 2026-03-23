import sys
input = sys.stdin.readline

class TreeNode:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

nums = []
while True:
    try:
        nums.append(int(input()))
    except:
        break

root = TreeNode(nums[0])
stack = [root]

for i in range(1, len(nums)):
    node = TreeNode(nums[i])

    if nums[i] < stack[-1].value:
        stack[-1].left = node
        stack.append(node)
    else:
        parent = None
        while stack and stack[-1].value < nums[i]:
            parent = stack.pop()
        parent.right = node
        stack.append(node)

def postorder_iterative(root):
    result = []
    stack = []
    last_visited = None
    current = root

    while stack or current:
        if current:
            stack.append(current)
            current = current.left
        else:
            peek = stack[-1]
            if peek.right and last_visited != peek.right:
                current = peek.right
            else:
                result.append(peek.value)
                last_visited = stack.pop()

    return result

result = postorder_iterative(root)
for x in result:
    print(x)