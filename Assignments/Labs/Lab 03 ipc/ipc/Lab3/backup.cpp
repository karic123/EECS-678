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
/* #define FIND_EXEC  "/bin/find" */ #define FIND_EXEC  "/usr/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

#include "RachUtils.h"

#include <iostream>
using namespace std;

enum STATE { TEST_PIPELINE, TEST_EXEC };
const STATE DEV_STATE = TEST_PIPELINE;    // Don't do exec stuff while working on pipeline

void Child_Find( pid_t* pid, int outputPipe[2], char* directory );
void Child_Grep( pid_t* pid, int inputPipe[2], int outputPipe[2], char* findme );
void Child_Sort( pid_t* pid, int inputPipe[2], int outputPipe[2] );
void Child_Head( pid_t* pid, int inputPipe[2], int outputPipe[2], int fileCount );
void DebugOut( int inputPipe[2] );

//int main( int argumentCount, char *arguments[] )
int main()
{
//	if ( argumentCount != 4 )
//	{
//		printf( "usage: finder DIR STR NUM_FILES\n" );
//		exit( 0 );
//	}

    // Test arguments
	char* arguments[] = { "x", "/home/rayechell/temp", "Question", "5" };

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

    // FIND

	findPid = fork();
	if ( IsChild( findPid ) )
	{
		Child_Find( &findPid, findGrepPipe, arguments[1] );
		exit( 0 ); // shouldn't be called because of execv
	}
	else
	{
        DebugOut( findGrepPipe );
	}

    // TODO: wait doesn't return at this point if called... investigate
//    wait( &status );
//    if ( status != NULL )
//    {
//        PrintStatus( status );
//    }


    // XARGS - GREP

//    grepPid = fork();
//    if ( IsChild( grepPid ) )
//    {
//        Child_Grep( &grepPid, findGrepPipe, grepSortPipe, arguments[2] );
//        exit( 0 ); // shouldn't be called because of execv
//    }

    DebugOut( grepSortPipe );

    printf( "Program end \n" );

	return 0;
}

void DebugOut( int inputPipe[2] )
{
    close( inputPipe[ WRITE_OUTPUT_PIPE ] );
    int bytes;
    char readBuffer[256];
    int counter = 0;
    do
    {
        bytes = read( inputPipe[ READ_INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );
        printf( "%i %i: %s \n", counter, sizeof( readBuffer ), readBuffer );
        counter++;
    } while ( bytes );
}

void Child_Find( pid_t* pid, int outputPipe[2], char* directory )
{
    if ( DEV_STATE == TEST_PIPELINE )
    {
        char* message = "Child - Find test \n";
        close( outputPipe[ READ_INPUT_PIPE ] );
        write( outputPipe[ WRITE_OUTPUT_PIPE ], message, sizeof( message ) );
    }
    else if ( DEV_STATE == TEST_EXEC )
    {
        // TODO: find $1 -name '*'.[ch] have it search only .c and .h files
        printf( "CHILD: Find() at directory \"%s\" \n", directory );

        // Clone the output from STD OUT as the input for this pipe
        close( outputPipe[ READ_INPUT_PIPE ] );
        dup2( outputPipe[ WRITE_OUTPUT_PIPE ], STDOUT_FILENO );

        char* programName = FIND_EXEC;
        char* arguments[] = { programName, directory, NULL };

        if ( execv( programName, arguments ) == -1 )
        {
            PrintError();
        }
    }
}

void Child_Grep( pid_t* pid, int inputPipe[2], int outputPipe[2], char* findme )
{
    if ( DEV_STATE == TEST_PIPELINE )
    {
    }
    else if ( DEV_STATE == TEST_EXEC )
    {
        printf( "CHILD: Grep() find text \"%s\" \n", findme );

        // Clone the output from input pipe as the input for this pipe
        close( outputPipe[ READ_INPUT_PIPE ] );
        dup2( outputPipe[ WRITE_OUTPUT_PIPE ], inputPipe[ WRITE_OUTPUT_PIPE ] );

        char* programName = XARGS_EXEC;
        char* arguments[] = { programName, GREP_EXEC, findme, NULL };

        if ( execv( programName, arguments ) == -1 )
        {
            PrintError();
        }
    }
}

void Child_Sort( pid_t* pid, int inputPipe[2], int outputPipe[2] )
{
    printf( "CHILD: Sort() \n" );
}

void Child_Head( pid_t* pid, int inputPipe[2], int outputPipe[2], int fileCount )
{
    printf( "CHILD: Head() \n" );
}
