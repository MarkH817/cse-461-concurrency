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

Node *start = NULL;

// global variables
int filesPerThread = 2;
int fileCount = 0;

void addData(int);

void concatLists(Node *, Node *);
int readFiles(int *, char * [], Node * []);

int main(int argc, char *argv[])
{
    FILE *fp;
    int numfile = 0;
    char buffer[128];
    int i = 0;
    int dat;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./collectM filename \n");
        exit(0);
    }

    if (!(fp = fopen(argv[1], "r")))
    {
        return 0;
    }

    while (fscanf(fp, "%s\n", buffer) != EOF)
    {
        numfile++;
    }

    // Save the total fileCount to global
    fileCount = numfile;

    // Will hold the chunks of data recorded
    // Each element is the pointer to a Linked List Node
    Node *subLists[numfile];

    char filename[numfile][128];
    fseek(fp, 0, SEEK_SET);

    while (fscanf(fp, "%s\n", filename[i]) != EOF)
    {
        i++;
    }

    fclose(fp);

    for (i = 0; i < numfile; i++)
    {
        sprintf(buffer, "../data/%s", filename[i]);
        if (!(fp = fopen(buffer, "r")))
        {
            return 0;
        }

        while (fscanf(fp, "%d\n", &dat) != EOF)
        {
            addData(dat);
        }

        fclose(fp);
    }

    printList(start);
    destroyList(start);
}

void addData(int dat)
{

    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = dat;
    newNode->next = start;
    start = newNode;
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

/**
 * Reads the files and creates a list for the data read.
 *
 * Relevant global variables:
 * - [Read-only] `filesPerThread`
 * - [Read-only] `fileCount`
 *
 * Parameters:
 * - [Read-only] `section` is the threadId determining what chunk of files will be read
 * - [Read-only] `filesnames` is the list of files to be read
 * - [Write-only] `results` is the list of results pointing to the head of a Linked List
 */
int readFiles(int *section, char *filenames[], Node *results[])
{
    return 1;
}