/* This program originates from a version available
 * from:
 * https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
 * */

#include"helper.h"
#include<time.h>

/* Declare lock variable */
volatile long lock = 1;
 
void * insertThread(Args * args)
{
	int i, begin, end;
	int (*f)() = &rand;
	Node **start;
	begin = args->arg0;
	end = args->arg1;
	start = args->arg2;
	srand(time(0));
	for (i = begin; i < end; i++)
        insertAtTheBeginning(start, ((*f)() % 99999) + 1, createNode());
	pthread_exit(NULL);
}

void destroyList(Node *start)
{
	Node *temp, *head; 
	head = start;
	temp = start;
	int count = 0;
	while (temp!=NULL)
	{
		head = temp->next;
		free(temp);
		temp = head;
		count++;
	}
	printf("Destroy %d nodes\n", count);
}

Node * createNode(void)
{
	return (Node*) malloc(sizeof(Node));
}


/* Function to insert a node at the beginning of a linked list */
void insertAtTheBeginning(Node **start_ref, int data, Node * ptr)
{
    ptr->data = data;
	 m_lock(&lock);
    ptr->next = *start_ref;
    *start_ref = ptr;
	 m_unlock(&lock);
}
 
/* Function to print nodes in a given linked list */
void printList(Node *start)
{
    Node *temp = start;
	 int count = 0;
	 if (temp == NULL)
	 	fprintf(stderr,"ERROR: empty list\n");
	 else
	 {
    	printf("\n");
    	while (temp!=NULL)
    	{
      	printf("%d ", temp->data);
        	temp = temp->next;
			count++;
			if (count % 16 == 0)
				printf("\n");
    	}
	 }
}
 
/* Bubble sort the given linked list */
void bubbleSort(Node *start)
{
    int swapped, i;
    Node *ptr1;
    Node *lptr = NULL;
 
    /* Checking for empty list */
    if (start == NULL)
        return;
 
    do
    {
        swapped = 0;
        ptr1 = start;
 
        while (ptr1->next != lptr)
        {
            if (ptr1->data > ptr1->next->data)
            { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}
 
/* function to swap data of two nodes a and b*/
void swap(Node *a, Node *b)
{
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

/* function to perform spin locking */
void m_lock(volatile long * lock)
{
		__asm__ __volatile__(
			"LOCK: xorq %r14, %r14;"
			"xchg %r14,(%rdi);"
			"andq %r14,%r14;"
			"jz LOCK;");
}

/* function to release lock */
void m_unlock(volatile long * lock)
{
		__asm__ __volatile__(
			"movq $1, (%rdi);");
}
