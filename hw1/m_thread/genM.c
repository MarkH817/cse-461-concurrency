/**
 * Team Members:
 *
 * - Mark Hernandez
 * - Adrian Ariffin
 * - Greg Nail
 *
 * ---
 * 
 * Solution:
 *
 *  Originally, the address to the variable `i` was being passed as the argument for `createFile`.
 * However, the variable `i` was being updated by the main thread and shared with all child threads, creating a data race.
 *
 * The solution involves creating an integer array `fileList` with a length as long as the number of threads.
 * Each element has the value written, and each element's address is passed as the argument instead of `i`.
 * This ensures that there are no shared variables among threads, thus removing the data race.
 *
 * ---
 *
 * Debugging Process Used for Solving the Issue:
 *
 * 1. First, we ran the `genM` program a couple of times to see where the randomness is located.
 *
 * 2. Statically analyze the code. Check for shared variables or pointers to shared variables.
 *
 * 3. (After seeing `&i` being passed to `createFile`) Print the value located at the `tid` pointer. Run the program a couple more times.
 *
 * 4. Implement the fix, as mentioned above.
 */

// Import libraries
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int createFile(int *); // Prototype function

int main(int argc, char *argv[])
{
	FILE *cp;				 // File pointer declaration
	char filename[64];		 // Declares space for a string of 64 characters
	int i, bound;			 // Declares variables for counter and number of files
	srand(time(0));			 // Seeds the random number generator using the unix epoch
	system("rm -rf ./data"); // Runs command to delete the `./data` directory
	system("mkdir ./data");  // Runs command to create an empty directory at `./data`

	if (argc < 3)
	{
		fprintf(stderr, "Usage: ./genM NumberOfFiles filename (e.g., ./genM 8 config.dat)\n"); // Prints the required arguments for an incorrect amount of arguments
		exit(0);																			   // Exits the program early using an error code of 0 signifying that the program ran properly
	}

	pthread_t thread[atoi(argv[1])];		 // Creates the array to hold thread references
	sprintf(filename, "./data/%s", argv[2]); // Writes the resulting string in the string (or char array) variable
	bound = atoi(argv[1]);					 // Converts string to integer

	int fileList[bound]; // Creates an integer array with the same length as the thread list

	// Checks if the file pointer was successful
	if (!(cp = fopen(filename, "w+")))
	{
		return 0; // Exits early if not available
	}

	for (i = 0; i < bound; i++)
	{
		// Set value to unique address to avoid sharing variables among threads
		fileList[i] = i;

		// Creates a new thread with the address of the arguments passed to it
		pthread_create(&thread[i], NULL, (void *)createFile, &fileList[i]);

		sprintf(filename, "%d.txt", i + 1); // Writes the resulting string in the string (or char array) variable
		fprintf(cp, "%s\n", filename);		// Writes to the disk by appending the file to the list
	}

	// Waits for each thread to finish
	for (i = 0; i < bound; i++)
	{
		pthread_join(thread[i], NULL);
	}

	fclose(cp);									// Close file write stream if the stream is closed successfully this should return a zero value but if it fails and EOF value will be returned.
	printf("Done creating %d files.\n", bound); // Print to system output
}

int createFile(int *tid)
{
	FILE *fp;									  // File pointer
	int rep;									  // Int variable to hold random numbers
	char filename[64];							  // Declares space for a string of 64 characters
	char datafilename[64];						  // Declares space for a string of 64 characters
	int (*p)() = &rand;							  // Function pointer to rand()
	sprintf(filename, "%d.txt", *tid + 1);		  // Writes the resulting string in the string (or char array) variable
	sprintf(datafilename, "./data/%s", filename); // Writes the resulting string in the string (or char array) variable
												  // `datafilename` holds the relative path of the file to be written F

	printf("filename = %s\n", datafilename); // Prints the name of the file currently being written

	// Checks if the file pointer was successful
	if (!(fp = fopen(datafilename, "w+")))
		return 0; // Exits early if not available with an return code of 0

	rep = ((*p)() % 50) + 1; // Call pointer to function call for random integer between [1, 50] inclusive

	// Writes up to 50 numbers into the file and will write upto the number value of Rep when reaching this line
	while (rep > 0)
	{
		fprintf(fp, "%d\n", ((*p)() % 99999) + 1); // Prints a random integer between [1, 10000] inclusive
		rep--;									   // Decrement the counter by 1
	}

	fclose(fp); // Close file write stream if the stream is closed successfully this should return a zero value but if it fails and EOF value will be returned.
	return 1;   // Exits the program with an return code of 1
}
