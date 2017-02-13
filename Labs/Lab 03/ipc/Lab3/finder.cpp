/*  Rachel J Morris, EECS 678, Lab 3, 2017   */

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

#include "RachUtils.h"

#include <iostream>
using namespace std;

void ClosePipes( int pipes[6], int except1, int except2 );
void WaitForProcesses( pid_t* pid1, pid_t* pid2, pid_t* pid3, pid_t* pid4 );

const int FIND_GREP_READ = 0;
const int FIND_GREP_WRITE = 1;
const int GREP_SORT_READ = 2;
const int GREP_SORT_WRITE = 3;
const int SORT_HEAD_READ = 4;
const int SORT_HEAD_WRITE = 5;

//int main( int argumentCount, char *arguments[] )
int main()
{
    // Create process IDs
    pid_t findPid;
    pid_t grepPid;
    pid_t sortPid;
    pid_t headPid;

    // Create pipes
    // pipes[0]     FIND_GREP_READ     read end    find -> grep    read by grep
    // pipes[1]     FIND_GREP_WRITE    write end   find -> grep    written by find
    // pipes[2]     GREP_SORT_READ     read end    grep -> sort    read by sort
    // pipes[3]     GREP_SORT_WRITE    write end   grep -> sort    written by grep
    // pipes[4]     SORT_HEAD_READ     read end    sort -> head    read by head
    // pipes[5]     SORT_HEAD_WRITE    write end   sort -> head    written by sort
    int pipes[6];

    // Establish pipeline
    pipe( pipes );
    pipe( pipes + 2 );
    pipe( pipes + 4 );

    findPid = fork();
    if ( IsChild( findPid ) )
    {
        // Replace process's STDOUT with the findToGrep pipe's WRITE
        dup2( pipes[ FIND_GREP_WRITE ], STDOUT_FILENO );

        // Can close all the pipes now
        ClosePipes( pipes, FIND_GREP_WRITE, -1 );

        // Execute command
        char* arguments[] = { "/bin/bash", "-c", "find bash-4.2 -name \'*\'.[ch]", NULL };
        execv( arguments[0], arguments );

        exit( 0 );
    }

    grepPid = fork();
    if ( IsChild( grepPid ) )
    {
        // Replace process's STDIN with read end of findToGrep pipe
        dup2( pipes[FIND_GREP_READ], STDIN_FILENO );

        // Replace process's STDOUT with write end of grepToSort pipe
        dup2( pipes[GREP_SORT_WRITE], STDOUT_FILENO );

        // Can close all the pipes now
        ClosePipes( pipes, FIND_GREP_READ, GREP_SORT_WRITE );

        char* arguments[] =  { "/bin/bash", "-c", "xargs grep -c execute", NULL };
        execv( arguments[0], arguments );

        exit( 0 );
    }

    sortPid = fork();
    if ( IsChild( sortPid ) )
    {
        // Replace process's STDIN with read end of grepToSort pipe
        dup2( pipes[GREP_SORT_READ], STDIN_FILENO );

        // Replace process's STDOUT with write end of sortToHead pipe
        dup2( pipes[SORT_HEAD_WRITE], STDOUT_FILENO );

        // Can close all the pipes now
        ClosePipes( pipes, GREP_SORT_READ, SORT_HEAD_WRITE );

        char* arguments[] = { "/bin/bash", "-c", "sort -t : +1.0 -2.0 --numeric --reverse" };
        execv( arguments[0], arguments );

        exit( 0 );
    }

    headPid = fork();
    if ( IsChild( headPid ) )
    {
        // Replace process's STDIN with the read end of grepToSort
        dup2( pipes[SORT_HEAD_READ], STDIN_FILENO );

        // Can close all the pipes now
        ClosePipes( pipes, SORT_HEAD_READ, -1 );

        char* arguments[] = { "/bin/bash", "-c", "head --lines=10" };
        execv( arguments[0], arguments );

        exit( 0 );
    }


    close( pipes[ FIND_GREP_READ ] );

    // Wait for the children to die
    WaitForProcesses( &findPid, &grepPid, &sortPid, &headPid );

	return 0;
}

void ClosePipes( int pipes[6], int except1, int except2 )
{
    for ( int i = 0; i < 6; i++ )
    {
        if ( i != except1 && i != except2 )
        {
            close( pipes[i] );
        }
    }
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
