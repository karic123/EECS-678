#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>

#define BSIZE 256

#define BASH_EXEC  "/bin/bash"
#define FIND_EXEC  "/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

int main( int argc, char *argv[] )
{
	// Establish a pipeline
	int fileDesc[2];
	pipe( fileDesc );
	
	int status;
	pid_t pid_1, pid_2, pid_3, pid_4;

	if ( argc != 4 ) {
		printf( "usage: finder DIR STR NUM_FILES\n" );
		exit( 0 );
	}

	pid_1 = fork();
	if ( pid_1 == 0 ) {
		/* First Child */
		char** files = NULL;
		execv( ".", files );
		
		exit( 0 );
	}

	pid_2 = fork();
	if ( pid_2 == 0 ) {
		/* Second Child */
		exit( 0 );
	}

	pid_3 = fork();
	if ( pid_3 == 0 ) {
		/* Third Child */
		exit( 0 );
	}

	pid_4 = fork();
	if ( pid_4 == 0 ) {
		/* Fourth Child */
		exit( 0 );
	}

	if ( ( waitpid( pid_1, &status, 0 ) ) == -1 ) {
		fprintf( stderr, "Process 1 encountered an error. ERROR%d", errno );
		return EXIT_FAILURE;
	}
	if ( ( waitpid( pid_2, &status, 0 ) ) == -1 ) {
		fprintf( stderr, "Process 2 encountered an error. ERROR%d", errno );
		return EXIT_FAILURE;
	}
	if ( ( waitpid( pid_3, &status, 0 ) ) == -1 ) {
		fprintf( stderr, "Process 3 encountered an error. ERROR%d", errno );
		return EXIT_FAILURE;
	}
	if ( ( waitpid( pid_4, &status, 0 ) ) == -1 ) {
		fprintf( stderr, "Process 4 encountered an error. ERROR%d", errno );
		return EXIT_FAILURE;
	}

	return 0;
}
