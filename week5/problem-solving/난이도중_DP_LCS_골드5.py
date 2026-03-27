# DP - LCS (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/9251
import sys
input=sys.stdin.readline

astrli = input().strip()
bstrli = input().strip()

dp = []
for i in range(len(astrli) + 1):
    row = []
    for j in range(len(bstrli) + 1):
        row.append(0)
    dp.append(row)

for i in range(1, len(astrli) + 1):
    for j in range(1, len(bstrli) + 1):
        if astrli[i - 1] == bstrli[j - 1]:
            dp[i][j] = dp[i - 1][j - 1] + 1
        else:
            if dp[i - 1][j] > dp[i][j - 1]:
                dp[i][j] = dp[i - 1][j]
            else:
                dp[i][j] = dp[i][j - 1]

print(dp[len(astrli)][len(bstrli)])