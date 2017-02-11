/* Reference:
 * http://tldp.org/LDP/lpg/node10.html#SECTION00721000000000000000
 * http://tldp.org/LDP/lpg/node11.html
 * 
 * "To send data to the pipe, we use the write() system call, 
 * and to retrieve data from the pipe, we use the read() system call."
 * */

// Happy C++ things
#include <iostream>
using namespace std;

#include <unistd.h>
#include <cstring>

void ChildProgram( int fileDescriptors[2] )
{
	char message[] = "I hate C strings\n";
	size_t messageLength = strlen( message ) + 1;
	
	// Child process - must close the "input" pipe
	close( fileDescriptors[0] );
	
	// Send data through the output pipe
	cout << "CHILD: Write message: \"" << message << "\" (" << messageLength << " bytes)" << endl;
	write( fileDescriptors[1], message, messageLength );
}

void ParentProgram( int fileDescriptors[2] )
{
	char readBuffer[80];
	
	// Parent process - must close the "output" pipe
	close( fileDescriptors[1] );
	
	// Receive data through the input pipe
	int byteCount = read( fileDescriptors[0], readBuffer, sizeof( readBuffer ) );
	cout << "PARENT: Received message: \"" << readBuffer << "\" (" << byteCount << " bytes)" << endl;
}

int main()
{	
	// Establish a pipeline
	int fileDescriptors[2];
	pipe( fileDescriptors );
	
	// Create a child process to execute a task
	pid_t childPid = fork();
	if ( childPid == -1 )
	{
		cerr << "Error with fork()" << endl;
		return -1;
	}
	else if ( childPid == 0 )
	{
		ChildProgram( fileDescriptors );
	}
	else
	{
		ParentProgram( fileDescriptors );
	}
	
	return 0;
}
