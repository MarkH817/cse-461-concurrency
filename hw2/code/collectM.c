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
 * - Sp = 0.25
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "helper/helper.h"

// global variables
int filesPerThread = 2;
int fileCount = 0;
char *files;
Node **listSections = NULL;

// Mutex
sem_t initMutex;
sem_t fileMutex;
sem_t memMutex;

int readFiles(void *);
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
    listSections = subLists;

    // Prepare threads
    pthread_t thread[threadCount];

    // Initialize mutex
    sem_init(&initMutex, 0, 1);
    sem_init(&fileMutex, 0, 1);
    sem_init(&memMutex, 0, 1);

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

    // Save filenames to global
    files = filename;

    sem_wait(&initMutex);
    for (i = 0; i < threadCount; i++)
    {
        subLists[i] = NULL;

        // Pass the readFiles function
        // Pass by value the integer between [0, threadCount)
        pthread_create(&thread[i], NULL, (void *)readFiles, (void *)i);
    }
    sem_post(&initMutex);

    // Wait for all threads to finish
    for (i = 0; i < threadCount; i++)
    {
        pthread_join(thread[i], NULL);
    }

    // Merge all lists into one main list
    for (i = 0; i < threadCount; i++)
    {
        if (i == 0) {
            list = subLists[i];
        } else {
            concatLists(subLists[i - 1], subLists[i]);
        }
    }

    // Free the memory and clear the list
    destroyList(list);

    return 0;
}


/**
 * Reads the files and creates a list for the data read.
 *
 * Relevant global variables:
 * - [Read-only] `filesPerThread`
 * - [Read-only] `fileCount`
 * - [Read-only] `files` is the list of files to be read
 * - [Write-only] `listSections` is the list of results pointing to the head of a 
Linked List
 *
 * Parameters:
 * - [Read-only] `tid` determines which chunk of files will be read
 */
int readFiles(void *tid)
{
    FILE *fp;
    char buffer[128];
    int data;
    Node *start = NULL;
    Node *newNode = NULL;

    int section = (int)tid;
    int offset = section * filesPerThread;
    int max = offset + filesPerThread;

    sem_wait(&initMutex);
    sem_post(&initMutex);

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

    // Read only a chunk of the files listed in the `files` array
    while (offset < max)
    {
        sprintf(buffer, "../data/%s", &(files[128 * offset]));

        sem_wait(&fileMutex);
        sem_wait(&memMutex);

        fp = fopen(buffer, "r");

        sem_post(&memMutex);
        sem_post(&fileMutex);

        // Exit early if file is not available
        if (!fp)
        {
            return 0;
        }

        while (fscanf(fp, "%d\n", &data) != EOF)
        {
            // Add data to local list
            sem_wait(&memMutex);
            newNode = (Node *)malloc(sizeof(Node));
            sem_post(&memMutex);

            if (newNode == NULL) {
                return 0;
            }

            newNode->data = data;
            newNode->next = start;
            start = newNode;
        }

        sem_wait(&memMutex);
        fclose(fp);
        sem_post(&memMutex);

        offset++;
    }

    // Save pointer to local list to result array `listSections`
    listSections[section] = start;

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

    // Traverse the base list until reaching the last node of that list
    while (temp != NULL)
    {
        next = temp->next;

        if (next == NULL)
        {
            // Reach last node
            // Point to the start of the next list
            temp->next = tail;
            return;
        }
        else
        {
            // Keep traversing
            temp = next;
        }
    }
}
