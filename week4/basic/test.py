# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
from collections import deque
class Solution(object):
    def averageOfLevels(self, root):
        """
        :type root: Optional[TreeNode]
        :rtype: List[float]
        """
        result=[]
        queue=deque()
        queue.append(root)

        while queue:
            level = len(queue)
            sum1 = 0
            for i in range(level):
                node = queue.popleft()
                sum1 += node.val
                if node.left:
                    queue.append(node.left)
                if node.right:
                    queue.append(node.right)
            avr=float(sum1) / level
            result.append(avr)
        return result
        