#include <iostream>
using namespace std;

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

#include "RachUtils.h"

void ChildProgram( int childToParent[2] )
{
    // The child process is going to write to the console,
    // and that console output will also be the parent's input
	char message[] = "I hate C strings\n";
	size_t messageLength = strlen( message ) + 1;

    // Duplicate - Child in << STDOUT
	close( childToParent[ READ_PIPE ] );
    //dup2( childToParent[ WRITE_PIPE ], STDOUT_FILENO );

	// Send data through the output pipe
	//printf( "CHILD: " );
	//printf( message );
}

void ParentProgram( int childToParent[2] )
{
	char readBuffer[80];

	// Parent process - must close the "output" pipe
	close( childToParent[ WRITE_PIPE ] );

	// Receive data through the input pipe
	int byteCount = read( childToParent[ READ_PIPE ], readBuffer, sizeof( readBuffer ) );

	if ( byteCount > 0 )
	{
        cout << endl << endl << "PARENT: Received message: \"" << readBuffer << "\" (" << byteCount << " bytes)" << endl;
	}
	else
	{
        cout << endl << endl << "PARENT: No information received." << endl;
	}
}

int main()
{
	// Establish a pipeline
	int childToParent[2];
	pipe( childToParent );

	// Create a child process to execute a task
	pid_t childPid = fork();
	if ( childPid == FORK_ERROR )
	{
		cerr << "Error with fork()" << endl;
		return FORK_ERROR;
	}
	else if ( childPid == CHILD_PROCESS )
	{
		ChildProgram( childToParent );
	}
	else
	{
        wait( NULL );
		ParentProgram( childToParent );
	}

	return 0;
}
