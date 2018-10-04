#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h> //importing libraries

int createFile(int *); // Prototype function

int main(int argc, char *argv[]) 
{
	FILE *cp; //File pointer declaration
	char filename[64]; //declares space for the filename
	int i, bound; //declares variables
	srand(time(0)); //seeds the random number generator using the unix epoch
	system("rm -rf ./data"); //runs commands to through the terminal
	system("mkdir ./data");//runs commands to through the terminal
	if (argc < 3) {
		fprintf(stderr,"Usage: ./genM NumberOfFiles filename (e.g., ./genM 8 config.dat)\n"); //prints the arguments for if argc is < 3
		exit(0); //exits the program
	}

	pthread_t thread[atoi(argv[1])]; // making a number of threads that is passed through the args.
	sprintf(filename, "./data/%s", argv[2]); //system print function
	bound = atoi(argv[1]); // converts strings to integer
	
	int fileList[bound]; // creates an integer array that has the same size as the bound variable

	if (!(cp = fopen(filename, "w+"))) { //if you cant open the file and write to it, return 0;
		return 0;
	}

	for (i = 0; i < bound; i++) {
		// Set value to unique address
		// to avoid sharing variables
		fileList[i] = i;

		pthread_create(&thread[i], NULL, (void *) createFile, &fileList[i]); // creates a new thread with the arguements passed to it
		sprintf(filename, "%d.txt", i + 1); //system print
		fprintf(cp,"%s\n", filename); // file print
	}

	for (i = 0; i < bound; i++) { 
		pthread_join(thread[i], NULL); // waits for each threads to finish.
	}

	fclose(cp); //closes the file
	printf("Done creating %d files.\n", bound); // print to system 
}

int createFile(int * tid) //int pointer
{
	FILE *fp; //file pointer
	int rep; // int variable
	char filename[64]; //char array with 64 spaces
	char datafilename[64]; //char array with 64 spaces
	int (*p)() = &rand; //function pointer that is &rand
	sprintf(filename, "%d.txt", *tid + 1); // system print the arguments given 
	sprintf(datafilename, "./data/%s", filename); // system print the arguments given 
	
	printf("filename = %s\n", datafilename); // prints the arguments
	if (!(fp = fopen(datafilename, "w+")))	//if no file, return 0
		return 0;
	rep = ((*p)() % 50) + 1; //pointer function call to be assigned to variable rep
	while (rep > 0) //while the rep is more than 0 
	{
		fprintf(fp,"%d\n", ((*p)() % 99999) + 1); //print file with the arguments
		rep--; //decrease rep
	}
	fclose(fp); //close file pointer
	return 1; //return 1
}
