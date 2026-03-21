# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
from collections import deque
class Solution(object):
    def maxDepth(self, root):
        """
        :type root: Optional[TreeNode]
        :rtype: int
        """
        
        queue=deque()
        queue.append(root)
        result=0
        while queue:
            count=len(queue)
            for i in range(count):
                x=queue.popleft()
                if x.left:
                    queue.append(x.left)
                if x.right:
                    queue.append(x.right)
            result+=1
        return result

        