/*
File name:      CS431_Morris_prog3.cpp
Author:         Rachel J. Morris
Date:           October 26, 2009

Description:    Program takes two arguments: Seconds for program to run and
                Number of work processes to create.

                Main will create the processes, and use the timer to
                decide when to kill the threads and exit the program.

                Each process will generated prime numbers between
                (i+1)*1000 and (i+2)*1000.  The function generating the
                prime numbers will stop outputting the results after
                50 primes are found.

                All of the processes will be generating output simultaneously.
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
#include <fstream>
using namespace std;

bool done;

void CalculatePrimes( int low, int high );
/*
    CalculatePrimes will calculate primes between the given low and high values.

    Input: Low, High
    Output: No return value
*/

static void HandleAlarm( int signo )
{
	cout<<"\n\n----------------------------------\n";
	cout<<"Alarm "<<signo<<": Ring ring ring!";
	cout<<"\n----------------------------------\n\n"<<endl;
	done = true;
}
/*
    HandleAlarm will output information and set a boolean to end program

    Input: signo, just displayed
    Output: no return value.
*/

int main( int argc, char *argv[] )
{
    /* Get arguments - seconds to run and amount of processes */
	if ( argc != 3 )
	{
		cout<<"\nNot enough arguments.  Include the amount of seconds to run and number of processes"<<endl;
		cout<<"\nExample: ./progname 3 15"<<endl;
		return 13;
	}

	int runLength = atoi( argv[1] );     // Set amount of seconds to run to the first argument
	int threadAmt = atoi( argv[2] );     // Set the amount of threads to create to the second argument

	cout<<"\n\nBeginning program with "<<runLength<<" seconds and "<<threadAmt<<endl;
	cout<<"Press a enter to start."<<endl;

	cin.get();

	done = false;

	// Set up alarm functionality
	struct sigaction act;
	memset( &act, 0, sizeof( act ) );
	act.sa_handler = &HandleAlarm;
	act.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &act, NULL);
	// Start alarm
	alarm( runLength );

	// Create processes
	// Dynamic array stores all the ids of the child processes so we can murder them later on.
	pid_t *childPID = new pid_t[threadAmt];
	for ( int i=0; i<threadAmt; i++ )
	{
        pid_t pID = fork();

        if ( pID == 0 )     // Child, start running the function
        {
            cout<<"\tBegun process #"<<i<<", pid: "<<getpid()<<endl;
            cout<<"\tGenerate primes between "<<(i+1)*1000<<" and "<<(i+2)*1000<<endl;

            CalculatePrimes( (i+1)*1000, (i+2)*1000 );

            break;
        }
        else    // Parent, store child's ID
        {
            childPID[i] = pID;
        }
	}

	// Continue looping until the alarm sets "done" to true.
	while ( !done );

	// Alarm has gone off; kill processes
	for ( int i=0; i<threadAmt; i++ )
	{
	    cout<<"\nKill child process #"<<i<<", id: "<<childPID[i]<<", parent id: "<<getppid()<<endl;
	    kill( childPID[i], SIGKILL );
	}

    // Free memory
	delete [] childPID;

    // Let user know program is over.
	cout<<"\nEnd of program\n";

    // Exit
	return 0;
}

void CalculatePrimes( int low, int high )
{
    bool prime;
    int current = low;
    int primeCount = 0;

    timespec mytime;
    mytime.tv_sec = 0;
    mytime.tv_nsec = 50000000; // (0.5 sec)

    for ( int i=low; i<high; i++ )
    {
        prime = true;

        // Check all numbers before i, excluding 0 and 1.
        for ( int j=2; j<i; j++ )
        {
            if ( i % j == 0 )
            {
                prime = false;
                break;
            }
        }

        if ( prime == true )
        {
            cout<<"\nPRIME: "<<i;
            primeCount++;
        }

        if ( primeCount >= 50 )
        {
            // Finished running function
            return;
        }

        nanosleep( &mytime, &mytime );
    }
}
