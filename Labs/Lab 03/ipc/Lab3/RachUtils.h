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

bool IsError( pid_t pid )
{
    return ( pid == -1 );
}

bool IsChild( pid_t pid )
{
    return ( pid == 0 );
}

int PrintError()
{
    printf( "\n ERROR! \t %s \n", strerror( errno ) );
    return errno;
}

void PrintStatus( int status )
{
    // http://man7.org/linux/man-pages/man2/waitpid.2.html

    if ( WIFEXITED( status ) )
    {
        printf( "child returned status %d - terminated normally with status %d \n", status, WEXITSTATUS( status ) );
    }

    else if ( WIFSIGNALED( status ) )
    {
       printf( "child returned status %d - killed by signal %d \n", status, WTERMSIG( status ) );
    }

    else if ( WIFSTOPPED( status ) )
    {
       printf( "child returned status %d - stopped by signal %d\n", status, WSTOPSIG( status ) );
    }

    else if ( WIFCONTINUED( status ) )
    {
       printf( "child returned status %d - continued\n", status );
    }
}

#endif
