/*
File name:      cs431_morris_producer.cpp
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

Description of cs431_morris_producer.cpp:
    The Producer reads in information from input.txt and after
    a random period of time, sends it to the Filter with the
    'prod-filt' pipe.
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
#include <ctype.h>
#include <fstream>
#include <cstdlib>
using namespace std;

int main()
{
    srand( time(NULL) );
    ifstream infile;
    infile.open( "input.txt" );
    string inbuffer;
    char buf[2];
    int i, fdpipe, randTime;

    /* Init pipe */
    if ( fdpipe = open( "prod-filt6", O_RDWR ) == -1 )
    {
        cout<<"Error opening prod-filt pipe"<<endl;
        return 4;
    }

    /* Read in a word at a time */
    while ( infile >> inbuffer )
    {
        // Parse each letter, changing character
        for ( i=0; i<inbuffer.size(); i++ )
        {
            buf[0] = inbuffer[i];
            buf[1] = '\0';

            // Send to pipe at random interval
            write( fdpipe, buf, strlen( buf ) );
            randTime = rand() % 10 + 1; // 1 - 10 seconds
            sleep( randTime );
        }
    }

    close( fdpipe );
    infile.close();

    return 0;

    /* Keep looping - the main program will kill this process */
    while ( 1 ) ;
}


















