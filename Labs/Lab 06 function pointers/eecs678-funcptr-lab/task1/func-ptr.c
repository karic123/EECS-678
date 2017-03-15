#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "process.h"
#include "util.h"

#define DEBUG 0			//change this to 1 to enable verbose output

/**
 * Signature for an function pointer that can compare
 * You need to cast the input into its actual
 * type and then compare them according to your
 * custom logic
 */
typedef int (*Comparer) (const void *a, const void *b);

/**
 * compares 2 processes
 * You can assume:
 * - Process ids will be unique
 * - No 2 processes will have same arrival time
 * Return 1, 0, -1
 * Sort based on arrival time (asc), then on priority (desc)
 */
int my_comparer(const void *this, const void *that)
{
	Process* ptr1 = (Process*)this;
	Process* ptr2 = (Process*)that;

	if ( ptr1->arrival_time < ptr2->arrival_time )
	{
		return -1;
	}
	else if ( ptr1->arrival_time > ptr2->arrival_time )
	{
		return 1;
	}

	// arrival times are the same, check priority
	if ( ptr1->priority < ptr2->priority )
	{
		return 1;	// (descending)
	}
	else if ( ptr1->priority > ptr2->priority )
	{
		return -1;
	}

	// Otherwise treat as same
	return 0;
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
		fflush(stdout);
		return 1;
	}

	/*******************/
	/* Parse the input */
	/*******************/
	FILE *input_file = fopen(argv[1], "r");
	if (!input_file) {
		fprintf(stderr, "Error: Invalid filepath\n");
		fflush(stdout);
		return 1;
	}

	Process *processes = parse_file(input_file);

	/*******************/
	/* sort the input  */
	/*******************/
	Comparer process_comparer = &my_comparer;

#if DEBUG
	for (int i = 0; i < P_SIZE; i++) {
		printf("%d (%d, %d) ",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
	}
	printf("\n");
#endif

	qsort(processes, P_SIZE, sizeof(Process), process_comparer);

	/**************************/
	/* print the sorted data  */
	/**************************/

	printf( "\n SORTED DATA - pid, arrival, priority \n" ); // debug
	for (int i = 0; i < P_SIZE; i++) {
		/* temp - restore plz
		printf("%d (%d, %d)\n",
				processes[i].pid,
				processes[i].priority, processes[i].arrival_time);
				*/
		printf("PID: %d \t Arrival: %d \t Priority: %d \n",
				processes[i].pid,
				processes[i].arrival_time, processes[i].priority);
	}
	fflush(stdout);
	fflush(stderr);

	/************/
	/* clean up */
	/************/
	free(processes);
	fclose(input_file);
	return 0;
}
