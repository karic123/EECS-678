
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void ExecuteA()
{
	printf( "\n Test 1: execl (“/bin/bash”, \"bash\", \"-c\", \"ls -ll\", (char *)NULL); \n" );
	
	pid_t pid = fork();
	if ( pid == 0 )
	{
		execl( "/bin/bash", "BANANA", "-c", "ls -ll", (char *)NULL );
		
		exit( 0 );
	}
	
	printf( "Hi I'm the parent \n" );
}

void ExecuteB()
{
	printf( "\n Test 2: execl (“/bin/ls”, \"ls\", \"-ll\", (char *)NULL); \n" );
	
	pid_t pid = fork();
	if ( pid == 0 )
	{
		execl( "/bin/ls", "ls", "-ll", (char *)NULL );
		
		exit( 0 );
	}
	
	printf( "Hi I'm the parent \n" );
}

int main()
{
	printf( "test 1 or test 2? " );
	int value = 0;
	scanf( "%d", &value );
	
	if ( value == 1 )
	{
		ExecuteA();
	}
	else
	{
		ExecuteB();
	}
	
	return 0;
}
