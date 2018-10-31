/**
 * Team Members:
 *
 * - Mark Hernandez
 * - Adrian Ariffin
 * - Greg Nail
 */

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

#define MIN_LEN_PARALLEL 1 << 7
#define USE_LOCKS 1

// Define sort arguments datatype
typedef struct SortArguments
{
	int start;
	int end;
	int *list;
} SortArgs;

// Function signatures
Node *mergeSortParallel(Node *);
int *nodeListToArray(Node *, int);
Node *arrayToNodeList(int *, int);
int nodeListCount(Node *);

int recursiveSort(SortArgs *);

void mergeSort(SortArgs *);
void merge(SortArgs *, SortArgs *);

int main(int argc, char *argv[])
{
	int items, threads, i, range, padding;

	if (argc != 1)
	{
		fprintf(stderr, "Usage: ./sortM \n");
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
	// bubbleSort(start);
	Node *previousList = start;
	start = mergeSortParallel(start);

	/* print list after sorting */
	printf("\n ===================== ");
	printf("\n Linked list after sorting ");
	printList(start);
	printf("\n");
	// destroyList(previousList);
	destroyList(start);

	return 0;
}

Node *mergeSortParallel(Node *list)
{
	int length = nodeListCount(list);
	int midwayPoint = length / 2;

	int *array = nodeListToArray(list, length);

	SortArgs argsLeft;
	argsLeft.list = array;
	argsLeft.start = 0;
	argsLeft.end = midwayPoint;

	SortArgs argsRight;
	argsRight.list = array;
	argsRight.start = midwayPoint;
	argsRight.end = length;

	pthread_t sortLeft;
	pthread_t sortRight;

	pthread_create(&sortLeft, NULL, (void *)recursiveSort, &argsLeft);
	pthread_create(&sortRight, NULL, (void *)recursiveSort, &argsRight);

	pthread_join(sortLeft, NULL);
	pthread_join(sortRight, NULL);

	// merge 2 lists
	merge(&argsLeft, &argsRight);

	Node *sortedList = arrayToNodeList(array, length);

	// Free the memory of the temporary array
	free(array);

	return sortedList;
}


/**
 * Convert the Node list into an array
 */
int *nodeListToArray(Node *list, int length)
{
	int *array = (int *)(malloc(sizeof(int) * length));
	int index = 0;
	Node *current = list;

	while (list != NULL && index < length)
	{
		array[index] = current->data;

		current = current->next;
		index++;
	}

	return array;
}

/**
 * Convert the array back into a Node list
 *
 * **Note**: Must insert from end to start
 */
Node *arrayToNodeList(int *array, int length)
{
	Node *start = NULL;
	Node *temp;
	int index = length - 1;

	while (index >= 0)
	{
		temp = (Node *)(malloc(sizeof(Node)));
		temp->next = start;
		temp->data = array[index];

		start = temp;
		index--;
	}
}

/**
 * Count the length of a Node list
 */
int nodeListCount(Node *list)
{
	int count = 0;
	Node *current = list;

	while (current != NULL)
	{
		count++;
		current = current->next;
	}

	return count;
}

/**
 * Recursively spawn new threads to sort a partition of the list
 * if the partition is long enough.
 *
 * Otherwise, sort the partition within the thread.
 */

int recursiveSort(SortArgs *args)
{
	int* list = args->list;
	int start = args->start;
	int end = args->end;

	int length = start - end;

	// If the list length is greater than the specified constant
	// Continue splitting via more threads
	if (length > MIN_LEN_PARALLEL)
	{
		int midpoint = (length / 2) + start;

		SortArgs left;
		left.list = list;
		left.start = start;
		left.end = midpoint;

		SortArgs right;
		right.list = list;
		right.start = midpoint;
		right.end = end;

		pthread_t sortLeft;
		pthread_t sortRight;

		pthread_create(&sortLeft, NULL, (void *)recursiveSort, &left);
		pthread_create(&sortRight, NULL, (void *)recursiveSort, &right);

		pthread_join(sortLeft, NULL);
		pthread_join(sortRight, NULL);

		// Merge the 2 sorted partitions
		merge(&left, &right);
	}
	else
	{
		// Perform a merge sort within the same thread
		mergeSort(args);
	}

	return 1;
}

// Sequential / regular version of the algorithm
void mergeSort(SortArgs *args)
{
	int *list = args->list;
	int start = args->start;
	int end = args->end;
	int length = end - start;
	int midpoint = length / 2 + start;

	// Early exit since a list length of 1 or less is sorted by definition
	if (length <= 1)
	{
		return;
	}

	SortArgs left;
	left.list = list;
	left.start = start;
	left.end = midpoint;

	SortArgs right;
	right.list = list;
	right.start = midpoint;
	right.end = end;

	// Recursively call for the 2 partitions
	mergeSort(&left);
	mergeSort(&right);

	// Merge the sorted partitions
	merge(&left, &right);
}

// Should sort the input array when merging
void merge(SortArgs *leftArgs, SortArgs *rightArgs)
{
	int *list = leftArgs->list;

	int leftStart = leftArgs->start;
	int leftEnd = leftArgs->end;
	int rightStart = rightArgs->start;
	int rightEnd = rightArgs->end;

	int length = rightEnd - leftStart;
	int currentLeft = leftStart;
	int currentRight = rightStart;

	int *sortedList = (int *)(malloc(sizeof(int) * length));
	int i = 0;

	if (sortedList == NULL) {
		fprintf(stderr, "Error\n");
		return;
	}

	// merge the lists
	while ((currentLeft < leftEnd) && (currentRight < rightEnd) && (i < length))
	{
		if (list[currentLeft] <= list[currentRight])
		{
			sortedList[i] = list[currentLeft];
			currentLeft++;
		}
		else
		{
			sortedList[i] = list[currentRight];
			currentRight++;
		}

		i++;
	}

	// Add the rest of the left sublist
	while ((currentLeft < leftEnd) && (i < length))
	{
		sortedList[i] = list[currentLeft];
		currentLeft++;
		i++;
	}

	// Add the rest of the right sublist
	while ((currentRight < rightEnd) && (i < length))
	{
		sortedList[i] = list[currentRight];
		currentRight++;
		i++;
	}

	// Copy back into original list
	for (i = 0; i < length; i++) {
		list[i + leftStart] = sortedList[i];
	}

	free(sortedList);
}
