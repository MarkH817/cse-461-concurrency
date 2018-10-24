/* This program originates from a version available
 * from:
 * https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
 * */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
 
/* structure for a node */
typedef struct dataNode
{
    int data;
    struct dataNode *next;
} Node;

typedef struct Arguments 
{
	int arg0;
	int arg1;
	Node ** arg2;
} Args;

/* Create a node to store data */
Node * createNode(void);

/* Function to insert values into the list 
 * using multiple threads */
void * insertThread(Args *args);
 
/* Function to insert a node at the beginning of a linked list */
void insertAtTheBeginning(Node **start_ref, int data, Node * ptr);
 
/* Function to bubble sort the given linked list */
void bubbleSort(Node *start);
 
/* Function to swap data of two nodes a and b*/
void swap(Node *a, Node *b);
 
/* Function to print nodes in a given linked list */
void printList(Node *start);

/* Function to free nodes in the list */
void destroyList(Node *start);

/* Function to perform spin locking */
void m_lock(volatile long * lock);

/* Function to release lock */
void m_unlock(volatile long * lock);
