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
#define FIND_EXEC  "/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

void ClosePipes( int pipes[6], int except1, int except2 );
void WaitForProcesses( pid_t* pid1, pid_t* pid2, pid_t* pid3, pid_t* pid4 );
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

    printf( " PARENT: Hey, my arguments are...: " );
    printf( "DIRECTORY: %s \t SEARCH TERM: %s \t HEAD COUNT: %s",
      arguments[ARG_DIRECTORY], arguments[ARG_SEARCH], arguments[ARG_HEAD]
      );
    printf( "\n\n enter something to continue... " );
    char blah;
    scanf( "%s", &blah );

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

    // Done with setup...

    findPid = fork();
    if ( IsChild( &findPid ) )
    {
        // Replace process's STDOUT with the findToGrep pipe's WRITE
        dup2( pipes[ FIND_GREP_WRITE ], STDOUT_FILENO );

        // Can close other pipes now
        ClosePipes( pipes, FIND_GREP_WRITE, -1 );

        char* findArgs[BSIZE];
        snprintf(findArgs, sizeof( findArgs ),
          "%s %s -name \'*\'.[ch]",
          FIND_EXEC,
          arguments[ ARG_DIRECTORY ] );

        // Execute command
        execl( BASH_EXEC,
          BASH_EXEC,
          "-c",
          findArgs,
          (char *)NULL );

        exit( 0 );
    }

    grepPid = fork();
    if ( IsChild( &grepPid ) )
    {
        // Replace process's STDIN with read end of findToGrep pipe
        dup2( pipes[FIND_GREP_READ], STDIN_FILENO );

        // Replace process's STDOUT with write end of grepToSort pipe
        dup2( pipes[GREP_SORT_WRITE], STDOUT_FILENO );

        // Can close other pipes now
        ClosePipes( pipes, FIND_GREP_READ, GREP_SORT_WRITE );

        char* grepArgs[BSIZE];
        snprintf(grepArgs, sizeof( grepArgs ),
          "%s %s -c %s",
          XARGS_EXEC,
          GREP_EXEC,
          arguments[ ARG_SEARCH ] );

        execl( BASH_EXEC,
          BASH_EXEC,
          "-c",
          grepArgs,
          (char *)NULL );

        exit( 0 );
    }

    sortPid = fork();
    if ( IsChild( &sortPid ) )
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

        char* sortArgs[BSIZE];
        snprintf(sortArgs, sizeof( sortArgs ),
          "%s -t : +1.0 -2.0 --numeric --reverse",
          SORT_EXEC );

        execl( BASH_EXEC,
          BASH_EXEC,
          "-c",
          sortArgs,
          (char *)NULL );
        exit( 0 );
    }


    headPid = fork();
    if ( IsChild( &headPid ) )
    {
        // Replace process's STDIN with the read end of grepToSort
        dup2( pipes[SORT_HEAD_READ], STDIN_FILENO );

        // Can close other now
        ClosePipes( pipes, SORT_HEAD_READ, -1 );

        char* headArgs[BSIZE];
        snprintf(headArgs, sizeof( headArgs ),
          "%s --lines=%s",
          HEAD_EXEC,
          arguments[ ARG_HEAD ] );

        char* arguments[] = { "/bin/bash", "-c", "head --lines=10", (char *)NULL };
        execl( BASH_EXEC,
          BASH_EXEC,
          "-c",
          headArgs,
          (char *)NULL );

        exit( 0 );
    }


    close( pipes[ FIND_GREP_READ ] );

    // Wait for the children to die
    //WaitForProcesses( &findPid, &grepPid, &sortPid, &headPid );

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

    if ( waitpid( *pid1, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid1, "find", status );
    }

    if ( waitpid( *pid2, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid2, "grep", status );
    }

    if ( waitpid( *pid3, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid3, "sort", status );
    }

    if ( waitpid( *pid4, &status, 0 ) == -1 )
    {
        PrintError();
    }
    else
    {
        PrintStatus( pid4, "head", status );
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
