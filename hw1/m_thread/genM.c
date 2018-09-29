#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

int createFile(int *);

int main(int argc, char *argv[])
{
	FILE *cp;
	char filename[64];
	int i, bound;
	srand(time(0));
	system("rm -rf ./data");
	system("mkdir ./data");
	if (argc < 3)
	{
		fprintf(stderr,"Usage: ./genM NumberOfFiles filename (e.g., ./genM 8 config.dat)\n");
		exit(0);
	}
	pthread_t thread[atoi(argv[1])];
	sprintf(filename, "./data/%s", argv[2]);
	bound = atoi(argv[1]);
	if (!(cp = fopen(filename, "w+")))
		return 0;
	for (i = 0; i < bound; i++) 
	{
		pthread_create(&thread[i], NULL, (void *) createFile, &i);
		sprintf(filename, "%d.txt", i + 1);
		fprintf(cp,"%s\n", filename);
	}
	for (i = 0; i < bound; i++) 
		pthread_join(thread[i], NULL);
	fclose(cp);
	printf("Done creating %d files.\n", bound);
}

int createFile(int * tid)
{
	FILE *fp;
	int rep;
	char filename[64];
	char datafilename[64];
	int (*p)() = &rand;
	sprintf(filename, "%d.txt", *tid + 1);
	sprintf(datafilename, "./data/%s", filename);
	//printf("filename = %s\n", datafilename);
	if (!(fp = fopen(datafilename, "w+")))
		return 0;
	rep = ((*p)() % 50) + 1;
	while (rep > 0)
	{
		fprintf(fp,"%d\n", ((*p)() % 99999) + 1);
		rep--;
	}
	fclose(fp);
	return 1;
}
