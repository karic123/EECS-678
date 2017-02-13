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

void Child_Find( pid_t* pid, int outputPipe[2], char* directory );
void Child_Grep( pid_t* pid, int inputPipe[2], int outputPipe[2], char* findme );
void Child_Sort( pid_t* pid, int inputPipe[2], int outputPipe[2] );
void Child_Head( pid_t* pid, int inputPipe[2], int outputPipe[2], int fileCount );
void DebugOut( int inputPipe[2] );

//int main( int argumentCount, char *arguments[] )
int main()
{
    // Process status
    int status;

    // Process IDs
    pid_t findPid;
    pid_t grepPid;
    pid_t sortPid;

    // Establish pipeline
    int findToGrep[2];
    pipe( findToGrep );
    int sortToGrep[2];
    pipe( sortToGrep );

    // find >>>>>> | >>>>>> grep >>>>>> | >>>>>> sort
    //        WR       RD          WR       RD
    findPid = fork();
    if ( IsChild( findPid ) )
    {
        close( findToGrep[ READ_INPUT_PIPE ] );

        char* message = "findpid \n";
        write( findToGrep[ WRITE_OUTPUT_PIPE ], message, sizeof( message ) );
        exit( 0 );
    }

    grepPid = fork();
    if ( IsChild( grepPid ) )
    {
        // This process WRITES to sortToGrep, and READS from findToGrep
        close( findToGrep[ WRITE_OUTPUT_PIPE ] );
        close( sortToGrep[ READ_INPUT_PIPE ] );

        // Redirect from findToGrep INPUT to sortToGrep OUTPUT
        dup2( sortToGrep[ READ_INPUT_PIPE ], findToGrep[ WRITE_OUTPUT_PIPE ] );

        char readBuffer[256];
        int bytes = read( findToGrep[ READ_INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );
        printf( "GREP CHILD Read: %i, %s \n", bytes, readBuffer );
        exit( 0 );
    }

    sortPid = fork();
    if ( IsChild( sortPid ) )
    {
        close( sortToGrep[ WRITE_OUTPUT_PIPE ] );

        char readBuffer[256];
        int bytes = read( sortToGrep[ READ_INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );
        printf( "SORT CHILD Read: %i, %s \n", bytes, readBuffer );
        exit( 0 );
    }



    // Wait for the children to die

    if ( waitpid( findPid, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( findPid, status );
    }

    if ( waitpid( grepPid, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( grepPid, status );
    }

    if ( waitpid( sortPid, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( sortPid, status );
    }

	return 0;
}

void DebugOut( int inputPipe[2] )
{
}

void Child_Find( pid_t* pid, int outputPipe[2], char* directory )
{
    printf( "CHILD: Find() at directory \"%s\" \n", directory );
}

void Child_Grep( pid_t* pid, int inputPipe[2], int outputPipe[2], char* findme )
{
    printf( "CHILD: Grep() find text \"%s\" \n", findme );
}

void Child_Sort( pid_t* pid, int inputPipe[2], int outputPipe[2] )
{
    printf( "CHILD: Sort() \n" );
}

void Child_Head( pid_t* pid, int inputPipe[2], int outputPipe[2], int fileCount )
{
    printf( "CHILD: Head() \n" );
}
