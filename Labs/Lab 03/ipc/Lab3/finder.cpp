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
