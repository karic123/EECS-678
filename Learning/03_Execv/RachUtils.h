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

#endif
