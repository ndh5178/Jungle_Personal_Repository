# class Solution(object):
#     def wordBreak(self, s, wordDict):
#         """
#         :type s: str
#         :type wordDict: List[str]
#         :rtype: bool
#         """
#         dp={}
#         for i in range(len(s)+1):
#             dp[i]=0
#         dp[0]=1
#         for i in range(1,len(s)+1):
#             for j in range(len(s)+1):
#                 if dp[j] == 1:
#                     if s[j:i] in wordDict:
#                         dp[i]=1
#                         break
        

#         if dp[len(s)]==1:
#             return True
#         else:
#             return False

# sol=Solution()
# print(sol.wordBreak("applepenapple",["apple","pen"]))

class Solution(object):
    def minimumTotal(self, triangle):
        """
        :type triangle: List[List[int]]
        :rtype: int
        """
        # dp={}
        # dp[0]=(triangle[0][0],0)

        # for i in range(1,len(triangle)):
        #     x,y=dp[i-1]
        #     if triangle[i][y]<triangle[i][y+1]:
        #         dp[i]=(x+triangle[i][y],y)
        #     else:
        #         dp[i]=(x+triangle[i][y+1],y+1)
        # return dp[len(triangle)-1][0]
        
        # def dfs(graph, start, visited=None):
        #     """
        #     깊이 우선 탐색 (재귀)
            
        #     Args:
        #         graph: 그래프 딕셔너리
        #         start: 현재 정점
        #         visited: 방문 리스트
            
        #     Returns:
        #         방문 순서 리스트
        #     """
        #     # TODO: visited가 None이면 초기화
        #     if visited == None:
        #         visited={}
        #     pass
            
        #     # TODO: 현재 정점 방문
        #     visited.append(start)
        #     pass
            
        #     # TODO: 인접한 정점들에 대해 재귀
        #     ## 방문하지 않은 정점이면 재귀 호출
        #     for i in graph[start]:
        #         if i not in visited:
        #             dfs(graph,i,visited)
        #     pass
            
        #     return visited
        # graph={}
        # for i in range(len(triangle)):
        #     for j in triangle[i]:
        #         if not graph[i]:
        #             graph[i]=[]
        #         if not graph[j]:
        #             graph[j]=[]
        #         graph[i].append(j)
        #         graph[j].append(i)
        # print(graph)
        # 

sol=Solution()
triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
print(sol.minimumTotal(triangle))