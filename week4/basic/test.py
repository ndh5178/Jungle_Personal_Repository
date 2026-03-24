from collections import deque

class Solution(object):
    def snakesAndLadders(self, board):
        """
        :type board: List[List[int]]
        :rtype: int
        """
        n = len(board)

        def get_position(num):
            row_from_bottom = (num - 1) // n
            col = (num - 1) % n

            row = n - 1 - row_from_bottom

            if row_from_bottom % 2 == 1:
                col = n - 1 - col

            return row, col

        visited = [False] * (n * n + 1)
        queue = deque()

        queue.append((1, 0))
        visited[1] = True

        while queue:
            current, count = queue.popleft()

            if current == n * n:
                return count

            dice = 1
            while dice <= 6:
                next_num = current + dice

                if next_num > n * n:
                    break

                r, c = get_position(next_num)

                if board[r][c] != -1:
                    destination = board[r][c]
                else:
                    destination = next_num

                if visited[destination] == False:
                    visited[destination] = True
                    queue.append((destination, count + 1))

                dice += 1

        return -1
    
board = [
    [-1,3,-1,-1,-1,-1],
    [-1,1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1],
    [-1,29,-1,-1,13,-1],
    [-1,-1,-1,-1,-1,-1],
    [-1,15,35,-1,-1,-1]
]

sol = Solution()
print(sol.snakesAndLadders(board))