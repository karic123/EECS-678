#ifndef _RACHELS_UTILS_HPP
#define _RACHELS_UTILS_HPP

#include <stdio.h>
#include <string.h>

/*
    write               read
    output <<< PIPE <<< input
*/

const int READ_INPUT_PIPE = 0;
const int WRITE_OUTPUT_PIPE = 1;

bool IsError( pid_t* pid )
{
    return ( *pid == -1 );
}

bool IsChild( pid_t* pid )
{
    return ( *pid == 0 );
}

int PrintError()
{
    printf( "\n ERROR! \t %s \n", strerror( errno ) );
    return errno;
}

void PrintStatus( pid_t* pid, int status )
{
    // http://man7.org/linux/man-pages/man2/waitpid.2.html

    if ( WIFEXITED( status ) )
    {
        printf( "child %i returned status %i - terminated normally with status %d \n", *pid, status, WEXITSTATUS( status ) );
    }

    else if ( WIFSIGNALED( status ) )
    {
       printf( "child %i returned status %i - killed by signal %d \n", *pid, status, WTERMSIG( status ) );
    }

    else if ( WIFSTOPPED( status ) )
    {
       printf( "child %i returned status %i - stopped by signal %d\n", *pid, status, WSTOPSIG( status ) );
    }

    else if ( WIFCONTINUED( status ) )
    {
       printf( "child %i returned status %i - continued\n", *pid, status );
    }
}

#endif
