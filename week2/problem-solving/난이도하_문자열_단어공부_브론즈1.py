# 문자열 - 단어 공부 (백준 브론즈1)
# 문제 링크: https://www.acmicpc.net/problem/1157

# 문제
# 알파벳 대소문자로 된 단어가 주어지면, 이 단어에서 가장 많이 사용된 알파벳이 무엇인지 알아내는 프로그램을 작성하시오. 단, 대문자와 소문자를 구분하지 않는다.

# 입력
# 첫째 줄에 알파벳 대소문자로 이루어진 단어가 주어진다. 주어지는 단어의 길이는 1,000,000을 넘지 않는다.

# 출력
# 첫째 줄에 이 단어에서 가장 많이 사용된 알파벳을 대문자로 출력한다. 단, 가장 많이 사용된 알파벳이 여러 개 존재하는 경우에는 ?를 출력한다.

def bj1157(string):
    string.upper()
    count = {}
    maxnum=0
    resertlist=[]
    for c in string:
        if c in count:
            count[c] += 1
        else:
            count[c] = 1
    for i in list(count.values()):
        if i>maxnum:
            maxnum=i
    
    for j, k in count.items():
        if k == maxnum:
            resertlist.append(j)

    if len(resertlist)>1:
        print("?")
    else:
        print(resertlist[0])

    

