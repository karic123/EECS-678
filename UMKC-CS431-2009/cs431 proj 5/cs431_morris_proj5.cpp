/*
File name:      cs431_morris_proj5.cpp
Author:         Rachel J. Morris
Date:           Due December 10th, 2009

Description of program:
    Three processes run together, piping information between each other.

    The Producer process reads data from a file and
    randomly sends it to the enxt program, the filter.

    The Filter will do a transformation based to the data
    (lowercase <-> uppercase) and randomly pass the data to the Consumer.

    The Consumer will read the data from the pipe, perform a
    reverse transformation on the data, an store the results
    in an output file.

Description of cs431_morris_proj5.cpp:
    Main program creates the pipes and starts the processes.
    Also takes care of the Alarm, which after a given amount
    of seconds (Given in the program arguments), will kill
    the processes.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <fstream>
using namespace std;

bool done;

const int PROC_AMT = 3;

enum Task { PRODUCER, FILTER, CONSUMER };

static void HandleAlarm( int signo )
{
    /*
    Argument:   Signal ID #
    Return:     None
    Function:   Set "done" flag to true so main leaves
                its loop and kills its processes...
    */
    cout<<"\n\nAlarm "<<signo<<": Ring ring ring!"<<endl;
    done = true;
}

int main( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
        cout<<"Need ONE argument after program name:"<<endl;
        cout<<"\t-Amount of seconds to run."<<endl;
        return 50;
    }

    done = false;
    umask( 0 );

    int parentID = getpid();
    int runLength = atoi( argv[1] );
    cout<<"Beginning program - Run for "<<runLength<<" seconds."<<endl;

    /* Set up alarm functionality */
	struct sigaction act;
	memset( &act, 0, sizeof( act ) );
	act.sa_handler = &HandleAlarm;
	act.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &act, NULL);
	// Start alarm
	alarm( runLength );

	/* Create pipes */
	string pipeName[2];
	int fifostat[2], fdpipe[2];
	pipeName[0] = "prod-filt7";
	pipeName[1] = "filt-cons7";

    for ( int i=0; i<2; i++ )
    {
        if ( fifostat[i] =   mkfifo( pipeName[i].c_str(), 0666 ) < 0 )
        {
            cout<<"Error mkfifo pipe "<<i<<", "<<pipeName[i]<<endl;
            close( fdpipe[0] );
            return 13;
        }
        if ( fdpipe[i] = open( pipeName[i].c_str(),   O_RDWR | O_NONBLOCK ) == -1 )
        {
            cout<<"Error opening pipe "<<i<<", "<<pipeName[i]<<endl;
            close( fdpipe[0] );
            close( fdpipe[1] );
            return 13;
        }

        cout<<"Created pipe "<<i<<": "<<pipeName[i]<<endl;
    }

    /* Create processes for Producer, Filter, and Consumer */
    pid_t *childPID = new pid_t[PROC_AMT];
    pid_t pID;
    Task task = PRODUCER;

    // Process 1 = Producer,    binary name "producer"
    // Process 2 = Filter,      binary name "filter"
    // Process 3 = Consumer,    binary name "consumer"

    for ( int i=0; i<PROC_AMT; i++ )
    {
        // Give process a task
        task = (Task)i;
        cout<<"Created process "<<i<<", id "<<getpid()<<", task is ";
        if ( task == PRODUCER )
        {
            cout<<"Producer"<<endl;
            // Create process
            pID = execve( "producer", NULL, NULL );
        }

        else if ( task == FILTER )
        {
            cout<<"Filter"<<endl;
            pID = execve( "filter", NULL, NULL );
        }

        else if ( task == CONSUMER )
        {
            cout<<"Consumer"<<endl;
            pID = execve( "consumer", NULL, NULL );
        }

        if ( pID != 0 )
        {
            // This is parent, store child ID
            childPID[i] = pID;
        }
    }

    if ( getpid() != parentID )
        return 100;

    /* Sit in loop until it's time to kill processes */
    while ( !done ) ;

    /* Outside of loop - kill processes */
    for ( int i=0; i<PROC_AMT; i++ )
    {
        cout<<"\nKilling process #"<<i<<endl;

        kill( childPID[i], SIGKILL );
    }

    /* Wait for terminated processes to respond */

    delete [] childPID;
    close( fdpipe[0] );
    close( fdpipe[1] );

    cout<<"\nEnd of program\n";

    return 0;
}






