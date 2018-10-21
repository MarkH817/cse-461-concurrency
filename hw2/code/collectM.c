/**
 * Team Members:
 *
 * - Mark Hernandez
 * - Adrian Ariffin
 * - Greg Nail
 *
 * ---
 *
 * Absolute Speed-Up:
 *
 * - [Insert result here]
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "helper/helper.h"

// global variables
int filesPerThread = 2;
int fileCount = 0;

int readFiles(void *, char *[], Node *[]);
void concatLists(Node *, Node *);

int main(int argc, char *argv[])
{
    FILE *fp;
    char buffer[128];
    int threadCount = 0;
    int i = 0;
    int dat;
    Node *list = NULL;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./collectM filename \n");
        exit(0);
    }

    // Checks if the input file exists
    if (!(fp = fopen(argv[1], "r")))
    {
        return 0;
    }

    // Counts the number of files listed in the input file
    while (fscanf(fp, "%s\n", buffer) != EOF)
    {
        fileCount++;
    }

    // Calculate the number of threads to use
    threadCount = fileCount / filesPerThread;

    // Increment threadCount to account for remainders
    if ((fileCount % filesPerThread) != 0)
    {
        threadCount++;
    }

    // Will hold the chunks of data recorded
    // Each element is the pointer to a Linked List Node
    Node *subLists[threadCount];

    // Prepare threads
    pthread_t thread[threadCount];

    // Reset the file pointer to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    // Save the filenames into an array of strings
    char filename[fileCount][128];

    // Read the entries until the end of the file
    // Each entry saved into the string array
    while (fscanf(fp, "%s\n", filename[i]) != EOF)
    {
        i++;
    }

    // Close the input file pointer
    fclose(fp);

    for (i = 0; i < threadCount; i++)
    {
        sublists[i] = NULL;
        pthread_create(&thread[i], NULL, (void *)readFiles, (void *)i, filename, sublists);
    }

    for (i = 0; i < threadCount; i++)
    {
        pthread_join(thread[i], NULL);

        if (i == 0)
        {
            list = sublists[i];
        }
        else
        {
            concatLists(sublists[i - 1], sublists[i]);
        }
    }

    printList(list);
    destroyList(list);
}


/**
 * Reads the files and creates a list for the data read.
 *
 * Relevant global variables:
 * - [Read-only] `filesPerThread`
 * - [Read-only] `fileCount`
 *
 * Parameters:
 * - [Read-only] `tid` determines which chunk of files will be read
 * - [Read-only] `filesnames` is the list of files to be read
 * - [Write-only] `results` is the list of results pointing to the head of a Linked List
 */
int readFiles(void *tid, char *filenames[], Node *results[])
{
    FILE *fp;
    char buffer[128];
    int data;
    Node *start = NULL;

    int section = (int)tid;
    int offset = section * filesPerThread;
    int max = offset + filesPerThread;

    // Exit early if given invalid section
    if (section < 0)
    {
        return 0;
    }

    // Ensure that the end doesn't extend pass the array
    if (max > fileCount)
    {
        max = fileCount;
    }

    while (offset < max)
    {
        sprintf(buffer, "../data/%s", filenames[offset]);

        if (!(fp = fopen(buffer, "r")))
        {
            return 0;
        }

        while (fscanf(fp, "%d\n", &data) != EOF)
        {
            // Add data to local list
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->data = data;
            newNode->next = start;
            start = newNode;
        }

        fclose(fp);

        offset++;
    }

    // Save pointer to local list to results array
    results[section] = start;

    return 1;
}

/**
 * Concat the LinkedLists into one list.
 *
 * The end of the `base` list shall point to the start of the `tail` list.
 */
void concatLists(Node *base, Node *tail)
{
    Node *temp = base;
    Node *next = NULL;

    while (temp != NULL)
    {
        next = temp->next;

        if (next == NULL)
        {
            temp->next = tail;
            return;
        }
        else
        {
            temp = next;
        }
    }
}
