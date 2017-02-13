/*  Rachel J Morris, EECS 678, Lab 3, 2017   */

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

void ClosePipes( int set1[2], int set2[2], int set3[2] );
void WaitForProcesses( pid_t* pid1, pid_t* pid2, pid_t* pid3, pid_t* pid4 );

//int main( int argumentCount, char *arguments[] )
int main()
{
    // Create process IDs
    pid_t findPid;
    pid_t grepPid;
    pid_t sortPid;
    pid_t headPid;

    // Create pipes
    // findToGrep[0]    read end    find -> grep    read by grep
    // findToGrep[1]    write end   find -> grep    written by find
    // grepToSort[0]    read end    grep -> sort    read by sort
    // grepToSort[1]    write end   grep -> sort    written by grep
    // sortToHead[0]    read end    sort -> head    read by head
    // sortToHead[1]    write end   sort -> head    written by sort
    int findToGrep[2];
    int grepToSort[2];
    int sortToHead[2];

    // Establish pipeline
    pipe( findToGrep );
    pipe( grepToSort );
    pipe( sortToHead );

    findPid = fork();
    if ( IsChild( findPid ) )
    {
        // Replace process's STDOUT with the findToGrep pipe's WRITE
        dup2( findToGrep[ 1 ], STDOUT_FILENO );

        // Can close all the pipes now
        ClosePipes( findToGrep, grepToSort, sortToHead );

        // Execute command
        char* arguments[] = { "/bin/bash", "-c", "find bash-4.2 -name \'*\'.[ch]", NULL };
        execv( arguments[0], arguments );

        exit( 0 );
    }

    grepPid = fork();
    if ( IsChild( grepPid ) )
    {
        // Replace process's STDIN with read end of findToGrep pipe
        dup2( findToGrep[ 0 ], STDIN_FILENO );

        // Replace process's STDOUT with write end of grepToSort pipe
        dup2( grepToSort[ 1 ], STDOUT_FILENO );

        // Can close all the pipes now
        ClosePipes( findToGrep, grepToSort, sortToHead );

        char* arguments[] =  { "/bin/bash", "-c", "xargs grep -c execute", NULL };
        execv( arguments[0], arguments );

        exit( 0 );
    }

    sortPid = fork();
    if ( IsChild( sortPid ) )
    {
        // Replace process's STDIN with read end of grepToSort pipe
        dup2( grepToSort[ 0 ], STDIN_FILENO );

        // Replace process's STDOUT with write end of sortToHead pipe
        dup2( sortToHead[ 1 ], STDOUT_FILENO );

        // Can close all the pipes now
        ClosePipes( findToGrep, grepToSort, sortToHead );

        char* arguments[] = { "/bin/bash", "-c", "sort -t : +1.0 -2.0 --numeric --reverse" };
        execv( arguments[0], arguments );

        exit( 0 );
    }

    headPid = fork();
    if ( IsChild( headPid ) )
    {
        // Replace process's STDIN with the read end of grepToSort
        dup2( sortToHead[ 0 ], STDIN_FILENO );

        // Can close all the pipes now
        ClosePipes( findToGrep, grepToSort, sortToHead );

        char* arguments[] = { "/bin/bash", "-c", "head --lines=10" };
        execv( arguments[0], arguments );

        exit( 0 );
    }



    // Parent: Close all pipes
    ClosePipes( findToGrep, grepToSort, sortToHead );

    // Wait for the children to die
    WaitForProcesses( &findPid, &grepPid, &sortPid, &headPid );

	return 0;
}

void ClosePipes( int set1[2], int set2[2], int set3[2] )
{
    close( set1[0] );
    close( set1[1] );
    close( set2[0] );
    close( set2[1] );
    close( set3[0] );
    close( set3[1] );
}

void WaitForProcesses( pid_t* pid1, pid_t* pid2, pid_t* pid3, pid_t* pid4 )
{
    // Create process status
    int status;

    if ( waitpid( *pid1, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid1, status );
    }

    if ( waitpid( *pid2, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid2, status );
    }

    if ( waitpid( *pid3, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid3, status );
    }

    if ( waitpid( *pid4, &status, NULL ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid4, status );
    }
}
