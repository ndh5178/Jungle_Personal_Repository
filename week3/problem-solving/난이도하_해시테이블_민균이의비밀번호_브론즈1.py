# 해시 테이블 - 민균이의 비밀번호 (백준 브론즈1)
# 문제 링크: https://www.acmicpc.net/problem/9933

#스택으로 풀어버렸다

# n=int(input())
# passward=[]

# for i in range(n):
#     passward.append(input())


# stack = []
# for i in range(n):
#     for j in passward[i]:
#         if j in stack:
#             stack.remove(j)
#             continue
#         stack.append(j)
#     if len(stack)==1:
#         print(len(passward[i]),stack[0])
#         break
#     stack = []
#     for k in range(i,n):

#         if passward[i]==passward[k][::-1]:
#             mid=len(passward[i])//2
#             print(len(passward[i]),passward[i][mid])
#             break


n=int(input())
passward=set()

for i in range(n):
    string=input()

    if string[::-1] in passward:
        mid=len(string)//2
        print(len(string),string[mid])
        break
    passward.add(string)

    if string == string[::-1]:
        mid=len(string)//2
        print(len(string),string[mid])
        break
    
    