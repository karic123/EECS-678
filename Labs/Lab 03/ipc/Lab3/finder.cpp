/*
Rachel J Morris, 2017
*/

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

void Child_Find( pid_t* pid, int findPipe[2], char* directory );
void Child_Grep( pid_t* pid, int findPipe[2], char* findme );
void Child_Sort( pid_t* pid, int findPipe[2] );
void Child_Head( pid_t* pid, int findPipe[2], int fileCount );

//int main( int argc, char *argv[] )
int main()
{
//	if ( argc != 4 )
//	{
//		printf( "usage: finder DIR STR NUM_FILES\n" );
//		exit( 0 );
//	}

    // Test arguments
	char* argv[] = { "x", "/home/rayechell/temp", "Question", "5" };

    // Process IDs
	pid_t findPid;
	pid_t grepPid;
	pid_t sortPid;
	pid_t headPid;

    // Pipes
	int findGrepPipe[2];
	int grepSortPipe[2];
	int sortHeadPipe[2];

	// Establish a pipeline
	pipe( findGrepPipe );
	pipe( grepSortPipe );
	pipe( sortHeadPipe );

    // Status
	int status;

	findPid = fork();
	if ( IsChild( findPid ) )
	{
		Child_Find( &findPid, findGrepPipe, argv[1] );
		exit( 0 );
	}

    wait( &status );

    cout << "Status: " << status << endl;

	return 0;
}

void Child_Find( pid_t* pid, int findPipe[2], char* directory )
{
    printf( "CHILD: Find() at directory \"%s\" \n", directory );

	close( findPipe[ READ_INPUT_PIPE ] );
    dup2( findPipe[ WRITE_OUTPUT_PIPE ], STDOUT_FILENO );

    char* programName = FIND_EXEC;
    char* arguments[] = { programName, directory, NULL };

    if ( execv( programName, arguments ) == -1 )
    {
        PrintError();
    }
}

void Child_Grep( pid_t* pid, int findPipe[2], char* findme )
{
    printf( "CHILD: Grep() \n" );
}

void Child_Sort( pid_t* pid, int findPipe[2] )
{
    printf( "CHILD: Sort() \n" );
}

void Child_Head( pid_t* pid, int findPipe[2], int fileCount )
{
    printf( "CHILD: Head() \n" );
}
