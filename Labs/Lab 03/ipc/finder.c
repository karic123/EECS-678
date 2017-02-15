/*  Rachel J Morris, EECS 678, Lab 3, 2017   */

/*
Desired output:
/bin/bash finder.sh bash-4.2 execute 10
bash-4.2/execute_cmd.c:187
bash-4.2/shell.c:49
bash-4.2/bashline.c:26
bash-4.2/builtins/evalstring.c:20
bash-4.2/trap.c:13
bash-4.2/subst.c:12
bash-4.2/eval.c:11
bash-4.2/builtins/evalfile.c:9
bash-4.2/flags.c:8
bash-4.2/variables.c:7

My output:
bash-4.2/shell.c:49
bash-4.2/bashline.c:26
bash-4.2/builtins/evalstring.c:20
bash-4.2/trap.c:13
bash-4.2/subst.c:12
bash-4.2/eval.c:11
bash-4.2/builtins/evalfile.c:9
bash-4.2/flags.c:8
bash-4.2/variables.c:7
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define BSIZE 256

#define BASH_EXEC  "/bin/bash"
#define FIND_EXEC  "/usr/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/usr/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

struct Process
{
    pid_t pid;
    char* name;
};

void ClosePipes( int pipes[6], int except1, int except2 );
void WaitForProcesses( struct Process* pid1, struct Process* pid2, struct Process* pid3, struct Process* pid4 );
int IsChild( pid_t* pid );
int PrintError();
void PrintStatus( pid_t* pid, char* name, int status );

const int FIND_GREP_READ = 0;
const int FIND_GREP_WRITE = 1;
const int GREP_SORT_READ = 2;
const int GREP_SORT_WRITE = 3;
const int SORT_HEAD_READ = 4;
const int SORT_HEAD_WRITE = 5;

const int ARG_DIRECTORY = 1;
const int ARG_SEARCH = 2;
const int ARG_HEAD = 3;

int main( int argumentCount, char *arguments[] )
{
    //char* arguments[] = { "...", "bash-4.2", "execute", "10" };

    // Create process IDs
    struct Process find, grep, sort, head;

    find.name = "find";
    grep.name = "grep";
    sort.name = "sort";
    head.name = "head";

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

    // Done with setup...

    find.pid = fork();
    if ( IsChild( &find.pid ) )
    {
        // Replace process's STDOUT with the findToGrep pipe's WRITE
        dup2( pipes[ FIND_GREP_WRITE ], STDOUT_FILENO );

        // Can close other pipes now
        ClosePipes( pipes, FIND_GREP_WRITE, -1 );

        char* findCommand[BSIZE];
        snprintf( findCommand, sizeof( findCommand ),
          "%s %s -name \'*\'.[ch]",
          FIND_EXEC,
          arguments[ ARG_DIRECTORY ] );

        printf( "Find args: " );
        printf( findCommand );

        // Execute command
        char* findArgs[] = { BASH_EXEC, "-c", findCommand, (char *)NULL };
        execv( findArgs[0], findArgs );

        exit( 0 );
    }

    grep.pid = fork();
    if ( IsChild( &grep.pid ) )
    {
        // Replace process's STDIN with read end of findToGrep pipe
        dup2( pipes[FIND_GREP_READ], STDIN_FILENO );

        // Replace process's STDOUT with write end of grepToSort pipe
        dup2( pipes[GREP_SORT_WRITE], STDOUT_FILENO );

        // Can close other pipes now
        ClosePipes( pipes, FIND_GREP_READ, GREP_SORT_WRITE );

        char* grepCommand[BSIZE];
        snprintf( grepCommand, sizeof( grepCommand ),
          "%s %s -c %s",
          XARGS_EXEC,
          GREP_EXEC,
          arguments[ ARG_SEARCH ] );

        char* grepArgs[] =  { BASH_EXEC, "-c", grepCommand, (char *)NULL };
        execv( grepArgs[0], grepArgs );

        exit( 0 );
    }

    sort.pid = fork();
    if ( IsChild( &sort.pid ) )
    {
        // Replace process's STDIN with read end of grepToSort pipe
        dup2( pipes[GREP_SORT_READ], STDIN_FILENO );

        // Replace process's STDOUT with write end of sortToHead pipe
        dup2( pipes[SORT_HEAD_WRITE], STDOUT_FILENO );

        // Can close other now
        ClosePipes( pipes, GREP_SORT_READ, SORT_HEAD_WRITE );

        char buffer[256];
        int bytes = read( pipes[ FIND_GREP_READ], buffer, sizeof( buffer ) );
        while ( bytes > 0 )
        {
          printf( buffer );
          bytes = read( pipes[ FIND_GREP_READ], buffer, sizeof( buffer ) );
        }

        char* sortCommand[BSIZE];
        snprintf( sortCommand, sizeof( sortCommand ),
          "%s -t : +1.0 -2.0 --numeric --reverse",
          SORT_EXEC );

        char* sortArgs[] = { BASH_EXEC, "-c", sortCommand, (char *)NULL };
        execv( sortArgs[0], sortArgs );

        exit( 0 );
    }


    head.pid = fork();
    if ( IsChild( &head.pid ) )
    {
        // Replace process's STDIN with the read end of grepToSort
        dup2( pipes[SORT_HEAD_READ], STDIN_FILENO );

        // Can close other now
        ClosePipes( pipes, SORT_HEAD_READ, -1 );

        char* headCommand[BSIZE];
        snprintf( headCommand, sizeof( headCommand ),
          "%s --lines=%s",
          HEAD_EXEC,
          arguments[ ARG_HEAD ] );

        char* headArgs[] = { BASH_EXEC, "-c", headCommand, (char *)NULL };
        execv( headArgs[0], headArgs );

        exit( 0 );
    }


    ClosePipes( pipes, -1, -1 );

    // Wait for the children to die
    int status;
    WaitForProcesses( &find, &grep, &sort, &head );

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

void WaitForProcesses( struct Process* pid1, struct Process* pid2, struct Process* pid3, struct Process* pid4 )
{
    // Create process status
    int status;

    printf( "\n Waiting on process %s... \n", (*pid1).name );
    if ( waitpid( (*pid1).pid, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( &(*pid1).pid, (*pid1).name, status );
    }

    printf( "\n Waiting on process %s... \n", (*pid2).name );
    if ( waitpid( (*pid2).pid, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( &(*pid2).pid, (*pid2).name, status );
    }

    printf( "\n Waiting on process %s... \n", (*pid3).name );
    if ( waitpid( (*pid3).pid, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( &(*pid3).pid, (*pid3).name, status );
    }

    printf( "\n Waiting on process %s... \n", (*pid4).name );
    if ( waitpid( (*pid4).pid, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( &(*pid4).pid, (*pid4).name, status );
    }
}

int IsChild( pid_t* pid )
{
    return ( *pid == 0 );
}

int PrintError()
{
    printf( "\n ERROR! \t %s \n", strerror( errno ) );
    return errno;
}

void PrintStatus( pid_t* pid, char* name, int status )
{
    // http://man7.org/linux/man-pages/man2/waitpid.2.html

    if ( WIFEXITED( status ) )
    {
        printf( "child %i '%s' returned status %i - terminated normally with status %d \n",
            *pid,
            name,
            status, WEXITSTATUS( status ) );
    }

    else if ( WIFSIGNALED( status ) )
    {
       printf( "child %i '%s' returned status %i - killed by signal %d \n",
        *pid,
        name,
        status, WTERMSIG( status ) );
    }

    else if ( WIFSTOPPED( status ) )
    {
       printf( "child %i '%s' returned status %i - stopped by signal %d\n",
        *pid,
        name,
        status, WSTOPSIG( status ) );
    }

    else if ( WIFCONTINUED( status ) )
    {
       printf( "child %i '%s' returned status %i - continued\n",
        *pid,
        name,
        status );
    }
}
