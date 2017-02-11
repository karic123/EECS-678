/* Reference:
* http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
 * */

// Happy C++ things
#include <iostream>
#include <fstream>
using namespace std;

#include <unistd.h>
#include <cstring>

#define FIND_EXEC  "/bin/find"

const int INPUT_PIPE 	= 0;
const int OUTPUT_PIPE 	= 1;

const int CHILD_PROCESS = 0;
const int FORK_ERROR = -1;

/* Parent       Child           execv
    pipe         pipe           stdout
    | <<<<<<<<<<< | <<<<<<<<<<<< |
     in       out   in       out
*/

void ChildProgram( int childToParent_fd[2], int execToChild_fd[2] )
{
    cout << "CHILD..." << endl;
    // dup2 = duplicate a file descriptor
    // CHILD pipeA_in <<<<<<<<<<< stdout EXECV
    dup2( execToChild_fd[ INPUT_PIPE ], STDOUT_FILENO );
	close( execToChild_fd[ OUTPUT_PIPE ] );

    // PARENT pipeB_in <<<<<<<<<<< pipeA_out CHILD
    dup2( childToParent_fd[ OUTPUT_PIPE ], execToChild_fd[ INPUT_PIPE ] );
	close( childToParent_fd[ INPUT_PIPE ] );

	char *args[] = { "find", "." };

    cout << "\t Child: Call exec" << endl;
	execv( FIND_EXEC, args );
}

void ParentProgram( int childToParent_fd[2] )
{
    cout << "PARENT..." << endl;
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
	int execToChild_fd[2];

	pipe( childToParent_fd );
	pipe( execToChild_fd );

	// Create a child process to execute a task
	pid_t pid = fork();
	if ( pid == FORK_ERROR )
	{
		cerr << "Error with fork()" << endl;
		return FORK_ERROR;
	}
	else if ( pid == CHILD_PROCESS )
	{
		ChildProgram( childToParent_fd, execToChild_fd );
	}
	else
	{
		ParentProgram( childToParent_fd );
	}

	return 0;
}
