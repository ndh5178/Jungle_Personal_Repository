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
    
    def BN(self,i,j):
        bnnode=self.head
        while bnnode.data!=i:
            bnnode=bnnode.next
        
        newnode=Node(j)
        cnnode=bnnode.next
        print(cnnode.data)
        bnnode.next=newnode
        newnode.next=cnnode

    def BP(self,i,j):
        bnnode=self.head
        knnode=self.head
        while bnnode.data!=i:
            bnnode=bnnode.next
        while knnode.next!=bnnode:
            knnode=knnode.next
        print(knnode.data)
        newnode=Node(j)
        cnnode=knnode.next
        knnode.next=newnode
        newnode.next=cnnode
        if cnnode == self.head:
            self.head = newnode

    def CN(self,i):
        bnnode=self.head
        while bnnode.data!=i:
            bnnode=bnnode.next
        cnnode=bnnode
        bnnode=bnnode.next
        print(bnnode.data)
        if bnnode == self.head:
            self.head = bnnode.next
        bnnode=bnnode.next
        cnnode.next=bnnode

    def CP(self,i):
        bnnode=self.head
        knnode=self.head
        lnnode=self.head
        while bnnode.data!=i:
            bnnode=bnnode.next
        while knnode.next!=bnnode:
            knnode=knnode.next
        while lnnode.next!=knnode:
            lnnode=lnnode.next
        
        cnnode=lnnode
        lnnode=lnnode.next
        print(lnnode.data)
        
        if lnnode == self.head:      
            self.head = lnnode.next
        
        lnnode=lnnode.next
        cnnode.next=knnode

    def linkedhead(self):
        if self.head is None:
            return

        current = self.head

        while current.next:
            current = current.next

        current.next = self.head






n,m=input().split()

li=LinkedList()
for i in map(int,input().split()):
    li.append(i)
li.linkedhead()

stack=[]

for i in range(int(m)):
    string=input().split()
    if len(string)==2:
        x=string.pop()
    if len(string)==3:
        y=string.pop()
        x=string.pop()    
    z=string.pop()
    if z=="BN":
        li.BN(int(x),int(y))
    if z=="BP":
        li.BP(int(x),int(y))
    if z=="CN":
        li.CN(int(x))
    if z=="CP":
        li.CP(int(x))





