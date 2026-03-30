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
    def wordBreak(self, s, wordDict):
        """
        :type s: str
        :type wordDict: List[str]
        :rtype: bool
        """
        dp={}
        for i in range(len(s)+1):
            dp[i]=0
        dp[0]=1
        for i in range(1,len(s)+1):
            if s[0:i] in wordDict:
                dp[i]=1
                start=i
        for j in range(start,len(s)+1):
            if s[start:j] in wordDict:
                dp[i]=1
                start=i
        if dp[len(s)]==1:
            return True
        else:
            return False

sol=Solution()
print(sol.wordBreak("catsandog",["cats","dog","sand","and","cat"]))