# 문자열 - IPv6 (백준 실버1)
# 문제 링크: https://www.acmicpc.net/problem/3107

T=input()
result=[]
if "::" in T:
    left,right = T.split("::")
    spleft=[]
    spright=[]
    if left != "":
        spleft=left.split(":")
    if right != "":
        spright=right.split(":")
    sumnum=8-(len(spleft)+len(spright))
    for i in spleft:
        result.append(i)
    for i in range(sumnum):
        result.append("0")
    for i in spright:
        result.append(i)
else:    
    result = T.split(":")
for i in range(len(result)):
    result[i] = result[i].zfill(4)
print(":".join(result))

