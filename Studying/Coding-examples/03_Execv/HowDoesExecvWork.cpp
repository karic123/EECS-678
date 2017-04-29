/* Reference:
* http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
 * */

// Happy C++ things
#include <iostream>
#include <fstream>
using namespace std;

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>


#define FIND_EXEC  "/usr/bin/find"
#define PWD_EXEC  "/bin/pwd"

#include "RachUtils.h"


void ChildProgram( int childToParent_fd[2] )
{
    // Duplicate - Child in << STDOUT
	close( childToParent_fd[ READ_INPUT_PIPE ] );
    dup2( childToParent_fd[ WRITE_OUTPUT_PIPE ], STDOUT_FILENO );

    // Execute the "pwd" command
    char* programName = FIND_EXEC;
    char* arguments[] = { programName, ".", NULL };

    if ( execv( programName, arguments ) == -1 )
    {
        PrintError();
    }
}

void ParentProgram( int childToParent_fd[2] )
{
	// Close output channel - just getting input from child
	close( childToParent_fd[ WRITE_OUTPUT_PIPE ] );

    char readBuffer[256];

    int bytes = read( childToParent_fd[ READ_INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );

    if ( bytes > 0 )
    {
        cout << "Parent - Received data: " << endl << endl
            << readBuffer << endl;
    }
    else
    {
        cout << "Parent - Did not receive any data!" << endl;
    }

}

int main()
{
	// Establish a pipeline
	int childToParent_fd[2];
	pipe( childToParent_fd );

	// Create a child process to execute a task
	pid_t pid = fork();

	if ( IsError( pid ) )
	{
		return PrintError();
	}
	else if ( IsChild( pid ) )
	{
		ChildProgram( childToParent_fd );
	}
	else
	{
        wait( NULL );
		ParentProgram( childToParent_fd );
	}

	return 0;
}
