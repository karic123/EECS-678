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

#define FIND_EXEC  "/bin/find"
#define PWD_EXEC  "/bin/pwd"

#include "RachUtils.h"

/* Parent       Child           execv
    pipe         pipe           stdout
    | <<<<<<<<<<< | <<<<<<<<<<<< |
     in       out   in       out
*/

void ChildProgram( int childToParent_fd[2] )
{
    // Duplicate - Child in << STDOUT
	close( childToParent_fd[ READ_PIPE ] );
    dup2( childToParent_fd[ WRITE_PIPE ], STDOUT_FILENO );

    // Execute the "pwd" command
    char* programName = PWD_EXEC;
    char* arguments[] = { programName, NULL };

    cout << "\t Child: Call exec" << endl;

    if ( execv( programName, arguments ) == -1 )
    {
        cerr << "\t Child: Error using exec" << endl;
        cout << GetExecvError( errno ) << endl;
    }

    cout << "\t Child: This shouldn't happen" << endl;

}

void ParentProgram( int childToParent_fd[2] )
{
	// Close output channel - just getting input from child
	close( childToParent_fd[ OUTPUT_PIPE ] );

    char readBuffer[256];

    int bytes = read( childToParent_fd[ INPUT_PIPE ], readBuffer, sizeof( readBuffer ) );
    cout << "\t Parent: Received data: " << readBuffer << endl;
}

int main()
{
	// Establish a pipeline
	int childToParent_fd[2];
	pipe( childToParent_fd );

	// Create a child process to execute a task
	pid_t pid = fork();
	if ( pid == FORK_ERROR )
	{
		cerr << "Error with fork()" << endl;
		return FORK_ERROR;
	}
	else if ( pid == CHILD_PROCESS )
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
