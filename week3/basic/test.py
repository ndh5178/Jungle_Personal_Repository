# 링크드리스트 - 철도 공사 (백준 골드4)
# 문제 링크: https://www.acmicpc.net/problem/23309
import sys
input = sys.stdin.readline

savedic={

}
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
        savedic[new_node.data]=lastnode

    
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
    
    def BN(self,i,j):
        crtnnode=self.head
        while crtnnode.data!=i:
            crtnnode=crtnnode.next
        
        newnode=Node(j)
        nextnode=crtnnode.next
        print(nextnode.data)
        crtnnode.next=newnode
        newnode.next=nextnode
        savedic[newnode.data]=crtnnode

    def BP(self,i,j):
        crtnnode=self.head
        prevnode=self.head
        while crtnnode.data!=i:
            crtnnode=crtnnode.next
        while prevnode.next!=crtnnode:
            prevnode=prevnode.next
        print(prevnode.data)
        newnode=Node(j)
        savenode=prevnode.next
        prevnode.next=newnode
        newnode.next=savenode
        savedic[newnode.data]=prevnode
        if savenode == self.head:
            self.head = newnode

    def CN(self,i):
        crtnnode=self.head
        while crtnnode.data!=i:
            crtnnode=crtnnode.next
        savenode=crtnnode
        crtnnode=crtnnode.next
        print(crtnnode.data)
        del(savedic[crtnnode.data])
        if crtnnode == self.head:
            self.head = crtnnode.next
        crtnnode=crtnnode.next
        savenode.next=crtnnode

    def CP(self,i):
        crtnnode=self.head
        prevnode=self.head
        prev2node=self.head
        while crtnnode.data!=i:
            crtnnode=crtnnode.next
        while prevnode.next!=crtnnode:
            prevnode=prevnode.next
        while prev2node.next!=prevnode:
            prev2node=prev2node.next
        
        savenode=prev2node
        prev2node=prev2node.next
        print(prev2node.data)
        del(savedic[prev2node.data])
        
        if prev2node == self.head:      
            self.head = prev2node.next
        
        prev2node=prev2node.next
        savenode.next=prev2node



n,m=input().split()

li=LinkedList()
for i in map(int,input().split()):
    li.append(i)
li.linkedhead()


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
        continue
    if z=="BP":
        li.BP(int(x),int(y))
        continue
    if z=="CN":
        li.CN(int(x))
        continue
    if z=="CP":
        li.CP(int(x))
        continue






