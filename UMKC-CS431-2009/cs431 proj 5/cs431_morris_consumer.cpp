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
using namespace std;

char Transform( char chara );

int main()
{
    char buf[2];
    int i, fdpipe, size;
    ofstream outfile;
    outfile.open( "output.txt" );

    /* Init pipe */
    if ( fdpipe = open( "filt-cons6", O_RDWR ) == -1 )
    {
        cout<<"Error opening filt-cons pipe"<<endl;
        return 5;
    }

    /* Read in info from pipe */
    do
    {
        size = read( fdpipe, buf, 2 );
        buf[size] = '\0';

        cout<<"read "<<buf<<endl;

        // Transform case
        buf[0] = Transform( buf[0] );

        // Write to output file
        outfile<<buf[0];

    } while ( size != 0 );

    close( fdpipe );
    outfile.close();

    return 0;

    /* Keep looping - the main program will kill this process */
    while ( 1 ) ;
}

char Transform( char chara )
{
    if ( int(chara) >= 97 && int(chara) <= 122 ) // lowercase character
    {
        chara = toupper( chara );
    }
    else if ( int(chara) >= 65 && int(chara) <= 90 ) // uppercase character
    {
        chara = tolower( chara );
    }

    return chara;
}

















