/* This program originates from a version available
 * from:
 * https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
 * */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "./helper/helper.h"

#define ITEMS 1 << 13
#define THREADS 1 << 3

int main(int argc, char *argv[])
{
	int items, threads, i, range, padding;

	if (argc != 1)
	{
		fprintf(stderr, "Usage: ./sortS \n");
		exit(0);
	}
	else
	{
		items = ITEMS;
		threads = THREADS;
	}

	/* start with empty linked list */
	Node *start = NULL;
	Args args[threads];

	range = items / threads;
	padding = items % threads;
	args[0].arg0 = 0;
	args[0].arg1 = args[0].arg0 + range + padding;
	args[0].arg2 = &start;

	for (i = 1; i < threads; i++)
	{
		args[i].arg0 = args[i - 1].arg1;
		args[i].arg1 = args[i].arg0 + range;
		args[i].arg2 = &start;
	}

	/* Create threads */
	pthread_t thread[threads];

	for (i = 0; i < threads; i++)
	{
		pthread_create(&thread[i], NULL, (void *)insertThread, &args[i]);
	}

	/* Join created threads to the main thread */
	for (i = 0; i < threads; i++)
	{
		pthread_join(thread[i], NULL);
	}

	/* print list before sorting */
	printf("\n Linked list before sorting");
	printList(start);

	/* sort the linked list */
	bubbleSort(start);

	/* print list after sorting */
	printf("\n ===================== ");
	printf("\n Linked list after sorting ");
	printList(start);
	printf("\n");
	destroyList(start);

	return 0;
}
