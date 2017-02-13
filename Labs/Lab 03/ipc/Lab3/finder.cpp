#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>

#define BSIZE 256

#define BASH_EXEC  "/bin/bash"
//#define FIND_EXEC  "/bin/find"
#define FIND_EXEC  "/usr/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

#include "RachUtils.h"

#include <iostream>
using namespace std;

void Child_Find( pid_t* pid, int fdChildToParent[2], char* directory );
void Child_Grep( pid_t* pid, int fdChildToParent[2], char* findme );
void Child_Sort( pid_t* pid, int fdChildToParent[2] );
void Child_Head( pid_t* pid, int fdChildToParent[2], int fileCount );

int main( int argc, char *argv2[] )
{
	// Establish a pipeline
	int fdChildToParent[2];
	int fdParentToChild[2];
	pipe( fdChildToParent );

	int status;
	pid_t pid_1, pid_2, pid_3, pid_4;

	char* argv[] = { "x", "/home/rayechell/temp", "Question", "5" };

	/*
	if ( argc != 4 ) {
		printf( "usage: finder DIR STR NUM_FILES\n" );
		exit( 0 );
	}
	* */


	pid_1 = fork();
	if ( IsChild( pid_1 ) )
	{
		Child_Find( &pid_1, fdChildToParent, argv[1] );
		exit( 0 );
	}

    wait( &status );
	close( fdChildToParent[ WRITE_OUTPUT_PIPE ] );

	pipe( fdParentToChild );
	close( fdParentToChild[ READ_INPUT_PIPE ] );
    //dup2( fdParentToChild[ WRITE_OUTPUT_PIPE ], fdChildToParent[ READ_INPUT_PIPE ] );

    /*


    while ( bytes > 0 )
    {
        bytes = read( fdChildToParent[ READ_INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );
        printf( "PARENT: Received %i bytes:\n", bytes );
        //printf( "%s \n\n", readBuffer );
    }
    */

    char readBuffer[256];
    int bytes;


	pipe( fdChildToParent );

	pid_2 = fork();
	if ( IsChild( pid_2 ) )
    {
        close( fdParentToChild[ READ_INPUT_PIPE ] );
        bytes = read( fdParentToChild[ READ_INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );
        printf( "CHILD: Received %i bytes:\n", bytes );

		exit( 0 );
	}


	/*

	pid_3 = fork();
	if ( pid_3 == 0 ) {
		exit( 0 );
	}

	pid_4 = fork();
	if ( pid_4 == 0 ) {
		exit( 0 );
	}
	*
	* */

	if ( ( waitpid( pid_1, &status, 0 ) ) == -1 ) {
		fprintf( stderr, "Process 1 encountered an error. ERROR%d", errno );
		return EXIT_FAILURE;
	}

	/*
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
	* */

	return 0;
}

void Child_Find( pid_t* pid, int fdChildToParent[2], char* directory )
{
    printf( "CHILD: Find() at directory \"%s\" \n", directory );

	close( fdChildToParent[ READ_INPUT_PIPE ] );
    dup2( fdChildToParent[ WRITE_OUTPUT_PIPE ], STDOUT_FILENO );

    char* programName = FIND_EXEC;
    char* arguments[] = { programName, directory, NULL };

    if ( execv( programName, arguments ) == -1 )
    {
        PrintError();
    }
}

void Child_Grep( pid_t* pid, int fdChildToParent[2], char* findme )
{
    printf( "CHILD: Grep() \n" );
}

void Child_Sort( pid_t* pid, int fdChildToParent[2] )
{
    printf( "CHILD: Sort() \n" );
}

void Child_Head( pid_t* pid, int fdChildToParent[2], int fileCount )
{
    printf( "CHILD: Head() \n" );
}
