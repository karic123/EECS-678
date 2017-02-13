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

//int main( int argumentCount, char *arguments[] )
int main()
{
    // Create process status
    int status;

    // Create process IDs
    pid_t findPid;
    pid_t grepPid;
    pid_t sortPid;

    // Create pipes
    // findToGrep[0]    read end    find -> grep    read by grep
    // findToGrep[1]    write end   find -> grep    written by find
    // sortToGrep[0]    read end    grep -> sort    read by sort
    // sortToGrep[1]    write end   grep -> sort    written by grep
    int findToGrep[2];
    int sortToGrep[2];

    // Establish pipeline
    pipe( findToGrep );
    pipe( sortToGrep );

    findPid = fork();
    if ( IsChild( findPid ) )
    {
        // Replace process's STDOUT with the findToGrep pipe's WRITE
        dup2( findToGrep[ 1 ], STDOUT_FILENO );

        // Can close all the pipes now
        close( findToGrep[ 0 ] );
        close( findToGrep[ 1 ] );
        close( sortToGrep[ 0 ] );
        close( sortToGrep[ 1 ] );

        // Execute command
        char* arguments[] = {"/bin/echo", "abcd", NULL};
        execv( arguments[0], arguments );

        exit( 0 );
    }

    grepPid = fork();
    if ( IsChild( grepPid ) )
    {
        // Replace process's STDIN with read end of findToGrep pipe
        dup2( findToGrep[ 0 ], STDIN_FILENO );

        // Replace process's STDOUT with write end of sortToGrep pipe
        dup2( sortToGrep[ 1 ], STDOUT_FILENO );

        // Can close all the pipes now
        close( findToGrep[ 0 ] );
        close( findToGrep[ 1 ] );
        close( sortToGrep[ 0 ] );
        close( sortToGrep[ 1 ] );

        char* arguments[] = {"/bin/grep", "a", NULL};
        execv( arguments[0], arguments );

        exit( 0 );
    }

    sortPid = fork();
    if ( IsChild( sortPid ) )
    {
        // Replace process's STDIN with the read end of sortToGrep
        dup2( sortToGrep[ 0 ], STDIN_FILENO );

        // Can close all the pipes now
        close( findToGrep[ 0 ] );
        close( findToGrep[ 1 ] );
        close( sortToGrep[ 0 ] );
        close( sortToGrep[ 1 ] );

        char* arguments[] = {"/usr/bin/cut", "-b", "1-10", NULL};
        execv( arguments[0], arguments );

        exit( 0 );
    }

    // Parent: Close all pipes
    close( findToGrep[ 0 ] );
    close( findToGrep[ 1 ] );
    close( sortToGrep[ 0 ] );
    close( sortToGrep[ 1 ] );

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
