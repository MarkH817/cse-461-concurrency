#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "helper/helper.h"

Node *start = NULL;

void addData(int);

int main(int argc, char *argv[])
{
	FILE *fp;
	//char filename[50][128];
	int numfile = 0;
	char buffer[128];
	int i = 0, dat;
	if (argc < 2)
	{
		fprintf(stderr, "Usage: ./collectS filename \n");
		exit(0);
	}
	if (!(fp = fopen(argv[1], "r")))
		return 0;
	while (fscanf(fp, "%s\n", buffer) != EOF)
		numfile++;
	char filename[numfile][128];
	fseek(fp, 0, SEEK_SET);
	while (fscanf(fp, "%s\n", filename[i]) != EOF)
		i++;
	fclose(fp);
	for (i = 0; i < numfile; i++)
	{
		sprintf(buffer, "../data/%s", filename[i]);
		if (!(fp = fopen(buffer, "r")))
			return 0;
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
