class Solution(object):
    def rob(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        # if len(nums)<2:
        #         return nums[0]
        # dp={}
        # dp[0]=nums[0]
        # dp[1]=nums[1]
        # dp[2]=nums[2]

        # for i in range(3,len(nums)):
        #     if dp[i-2]>dp[i-3]:
        #         dp[i]=dp[i]+dp[i-2]
        #     else:
        #         dp[i]=dp[i]+dp[i-3]
        # return max(dp)
            
        if len(nums)<2:
            return nums[0]
        dp={}
        dp[0]=nums[0]
        dp[1]=nums[1]

        for i in range(2,len(nums)):
            if i % 2 == 0:
                dp[1]+=nums[i]
            else:
                dp[2]+=nums[i]
        if dp[1]>dp[2]:
            return dp[1]
        else:
            return dp[2]