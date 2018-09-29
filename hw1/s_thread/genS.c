#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char *argv[])
{
	FILE *fp, *cp;
	char buffer[128];
	char filename[32];
	char datafilename[32];
	int bound;
	int i, rep, val;
	int (*p)() = &rand;;
	system("rm -rf ./data");
	system("mkdir ./data");
	srand(time(0));
	if (argc < 3)
	{
		fprintf(stderr,"Usage: ./genS NumberOfFiles Filename (e.g., ./genS 8 config.dat)\n");
		exit(0);
	}
	sprintf(filename, "./data/%s", argv[2]);
	if (!(cp = fopen(filename, "w+")))
	{
		fprintf(stderr, "Unable to create files. Check if your directory exists.\n");
		return 0;
	}
	bound = atoi(argv[1]);
	for (i = 0; i < bound; i++) 
	{
		sprintf(filename, "%d.txt", i+1);
		fprintf(cp,"%s\n", filename);
		sprintf(datafilename, "./data/%s", filename);
		if (!(fp = fopen(datafilename, "w+")))
			return 0;
		rep = ((*p)() % 50) + 1;
		while (rep > 0)
		{
			val = ((*p)() % 99999) + 1;
			fprintf(fp,"%d\n", val);
			rep--;
		}
		fclose(fp);
	}
	fclose(cp);
	printf("Done creating %d files\n", bound);
}
