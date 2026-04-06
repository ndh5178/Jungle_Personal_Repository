//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section C - Stack and Queue Questions
목적: 문제 1에 필요한 함수를 구현합니다. */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode
{
	int item;
	struct _listnode *next;
} ListNode;	// ListNode의 정의는 변경하면 안 됩니다

typedef struct _linkedlist
{
	int size;
	ListNode *head;
} LinkedList;	// LinkedList의 정의는 변경하면 안 됩니다


typedef struct _queue
{
	LinkedList ll;
} Queue;  // Queue의 정의는 변경하면 안 됩니다

///////////////////////// 함수 원형 선언 ////////////////////////////////////

// 이 함수들의 원형은 변경하면 안 됩니다
void createQueueFromLinkedList(LinkedList *ll, Queue *q);
void removeOddValues(Queue *q);

void enqueue(Queue *q, int item);
int dequeue(Queue *q);
int isEmptyQueue(Queue *q);
void removeAllItemsFromQueue(Queue *q);

void printList(LinkedList *ll);
ListNode * findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);

//////////////////////////// main() //////////////////////////////////////////////

int main()
{
	int c, i;
	LinkedList ll;
	Queue q;

	c = 1;

	// 연결 리스트를 빈 리스트로 초기화
	ll.head = NULL;
	ll.size = 0;

	// 큐를 빈 큐로 초기화
	q.ll.head = NULL;
	q.ll.size = 0;


	printf("1: Insert an integer into the linked list:\n");
	printf("2: Create the queue from the linked list:\n");
	printf("3: Remove odd numbers from the queue:\n");
	printf("0: Quit:\n");


	while (c != 0)
	{
		printf("Please input your choice(1/2/3/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to insert into the List: ");
			scanf("%d", &i);
			insertNode(&ll, ll.size, i);
			printf("The resulting linked list is: ");
			printList(&ll);
			break;
		case 2:
			createQueueFromLinkedList(&ll, &q); // 이 함수는 직접 구현해야 합니다
			printf("The resulting queue is: ");
			printList(&(q.ll));
			break;
		case 3:
			removeOddValues(&q); // 이 함수는 직접 구현해야 합니다
			printf("The resulting queue after removing odd integers is: ");
			printList(&(q.ll));
			removeAllItemsFromQueue(&q);
			removeAllItems(&ll);
			break;
		case 0:
			removeAllItemsFromQueue(&q);
			removeAllItems(&ll);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}

	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////

void createQueueFromLinkedList(LinkedList *ll, Queue *q)
{
    ListNode *temp, *newNode, *cur;

    removeAllItemsFromQueue(q);

    temp = ll->head;

    while (temp != NULL)
    {
        newNode = malloc(sizeof(ListNode));
        newNode->item = temp->item;
        newNode->next = NULL;

        if (q->ll.head == NULL)
        {
            q->ll.head = newNode;
			cur = newNode;
        }
        else
        {
            cur->next = newNode;
			cur=cur->next;
        }

        q->ll.size++;
        temp = temp->next;
    }
}


void removeOddValues(Queue *q)
{
	/* 여기에 코드를 작성하세요 */
	ListNode *save,*prev=NULL,*freenode;
	save=q->ll.head;
	while (save!=NULL)
	{
		if(save->item%2==1){
			freenode=save;
			if(prev ==NULL){
				q->ll.head=save->next;
			}
			else{
				prev->next = save->next;
			}
			save=save->next;
			free(freenode);
			q->ll.size--;
		}
		else{
			prev=save;
			save=save->next;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

void enqueue(Queue *q, int item) {
	insertNode(&(q->ll), q->ll.size, item);
}

int dequeue(Queue *q) {
	int item;

	if (!isEmptyQueue(q)) {
		item = ((q->ll).head)->item;
		removeNode(&(q->ll), 0);
		return item;
	}
	return -1;
}

int isEmptyQueue(Queue *q) {
	if ((q->ll).size == 0)
		return 1;
	return 0;
}

void removeAllItemsFromQueue(Queue *q)
{
	int count, i;
	if (q == NULL)
		return;
	count = q->ll.size;

	for (i = 0; i < count; i++)
		dequeue(q);
}


void printList(LinkedList *ll){

	ListNode *cur;
	if (ll == NULL)
		return;
	cur = ll->head;
	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item);
		cur = cur->next;
	}
	printf("\n");
}


void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL){
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
}


ListNode * findNode(LinkedList *ll, int index){

	ListNode *temp;

	if (ll == NULL || index < 0 || index >= ll->size)
		return NULL;

	temp = ll->head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1;

	// 리스트가 비어 있거나 첫 노드를 삽입하는 경우 head 포인터를 갱신해야 함
	if (ll->head == NULL || index == 0){
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		if (ll->head == NULL)
		{
			exit(0);
		}
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// 대상 위치의 이전 노드와 해당 위치의 노드를 찾음
	// 새 노드를 만들고 링크를 다시 연결함
	if ((pre = findNode(ll, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		if (pre->next == NULL)
		{
			exit(0);
		}
		pre->next->item = value;
		pre->next->next = cur;
		ll->size++;
		return 0;
	}

	return -1;
}


int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// 삭제할 수 있는 가장 큰 인덱스는 size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// 첫 번째 노드를 삭제하는 경우 head 포인터를 갱신해야 함
	if (index == 0){
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;
		return 0;
	}

	// 대상 위치의 이전 노드와 다음 노드를 찾음
	// 대상 노드를 해제하고 링크를 다시 연결함
	if ((pre = findNode(ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return -1;

		cur = pre->next;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 0;
	}

	return -1;
}

