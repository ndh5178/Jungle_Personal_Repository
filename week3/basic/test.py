# 링크드리스트 - 철도 공사 (백준 골드4)
# 문제 링크: https://www.acmicpc.net/problem/23309
import sys
input = sys.stdin.readline

class Node:
    def __init__(self, data):
        self.data = data
        self.next = None
class LinkedList:
    def __init__(self):
        self.head = None
    
    def append(self, data):
        new_node = Node(data)

        if self.head == None:
            self.head=new_node
            return
        
        lastnode=self.head

        while lastnode.next != None:
            lastnode=lastnode.next
        lastnode.next=new_node

    
    def print_list(self):
        values = []
        prtnode=self.head
        while prtnode != None:
            values.append(prtnode.data)
            prtnode=prtnode.next
            if prtnode==self.head: #원형루프 종료
                break
        return values
    
    def linkedhead(self):
        if self.head is None:
            return

        current = self.head

        while current.next:
            current = current.next

        current.next = self.head
    
    def L(self):
        global curser
        if curser == 0:
            return
        curser-=1

    def D(self):
        global curser
        global knum
        if curser == knum:
            return
        curser+=1

    def B(self):
        global curser
        cursernode=self.head
        acursernode=self.head
        for i in range(curser):
            cursernode=cursernode.next
        while acursernode.next !=cursernode:
            acursernode=acursernode.next
        savenode=acursernode
        acursernode=acursernode.next
        
        if acursernode == self.head:
            self.head = acursernode.next

        acursernode=acursernode.next
        savenode.next=acursernode

    def P(self,s):
        global curser
        newdata=Node(s)
        cursernode=self.head
        for i in range(curser):
            cursernode=cursernode.next
        if cursernode.next == None:
            cursernode.next=newdata
        acursernode=cursernode.next
        cursernode.next=newdata
        newdata.next=acursernode



li=LinkedList()
k=input().split()
for i in k:
    li.append(i)
n=int(input())
knum=len(k)
curser=knum
li.linkedhead()
li.print_list()
for i in range(n):
    string=input().split()
    if len(string)==2:
        x=string.pop()
    if len(string)==3:
        y=string.pop()
        x=string.pop()    
    z=string.pop()
    if z=="L":
        li.L()
    if z=="D":
        li.D()
    if z=="B":
        li.B()
    if z=="P":
        li.P(x)

li.print_list()





