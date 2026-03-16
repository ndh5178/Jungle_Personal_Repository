# 스택 - 탑 (백준 골드5)
# 문제 링크: https://www.acmicpc.net/problem/2493

# import sys
# input = sys.stdin.readline

# num=int(input())
# arr=list(map(int,input().split()))
# result=[]

# reversarr=arr[::-1]

# i=0
# j=1

# while i<len(reversarr)-1:
#     if j==len(reversarr)-1:
#         result.append(0)
#         i+=1
#         j=i+1
#         continue
#     if reversarr[i]<=reversarr[j]:
#         result.append((len(reversarr)-j))
#         i+=1
#         j=i+1
#         continue
#     j+=1


# result.append(0)
# result=result[::-1]
# result=list(map(str,result))
# print(' '.join(result))

###이건 생각은 좋았다고 하지만 여전히 시간 오류가 난다

# while top:
#     if len(savetop) == 0:
#         savetop.append(top.pop(0))
#         result.append(0)
#     curtop = top.pop(0)
#     vstop = savetop.pop()
#     if curtop[0] > vstop[0]:
#         while savetop:
#             vstop = savetop.pop()
#             if curtop[0] < vstop[0]:
#                 savetop.append(vstop)
#                 savetop.append(curtop)
#                 result.append(vstop[1])
#                 break
#         if len(savetop) == 0:
#             savetop.append(curtop)
#             result.append(0)
#             continue
#     if curtop[0] <= vstop[0]:
#         savetop.append(vstop)
#         savetop.append(curtop)
#         result.append(vstop[1])

# enumerate

import sys

input = sys.stdin.readline

num = int(input())
iptop = list(map(int, input().split()))
top = []
savetop = []
result = []

for idx, topnum in enumerate(iptop):
    if len(savetop) == 0:
        savetop.append((topnum, idx + 1))
        result.append(0)
        continue
    vstop = savetop.pop()
    if topnum > vstop[0]:
        while savetop:
            vstop = savetop.pop()
            if topnum < vstop[0]:
                savetop.append(vstop)
                savetop.append((topnum, idx + 1))
                result.append(vstop[1])
                break
        if len(savetop) == 0:
            savetop.append((topnum, idx + 1))
            result.append(0)
            continue
        continue
    if topnum <= vstop[0]:
        savetop.append(vstop)
        savetop.append((topnum, idx + 1))
        result.append(vstop[1])
result = map(str, result)
result = " ".join(result)
print(result)
