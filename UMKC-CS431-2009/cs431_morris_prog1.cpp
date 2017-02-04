/**********************************************************
*
* File Name: cs431_morris_prog1.cpp
* Author: Rachel J. Morris
* Date: September 2009
*
* Write a program in C++ to first display the following:
*
*   1 - GENERAL INFORMATION
* a) Machine/Host name
* b) System date & time
* c) Kernel version
* d) Memory usage
*
*   2 - PERFORMANCE INFORMATION
* a) Load in system idle time and uptime
* b) Run a function to keep the system busy
* c) Load in new system idle time and uptime
* d) Calculate change in up/idle times, calculate system utilization
*
************************************************************/

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <string>
#include <sys/time.h>
using namespace std;

void Work( char *name );

int main()
{
    // 1. Display general information
    //      a. Machine name (hostname)
    //      b. System date and time
    //      c. Kernel version
    //      d. Memory usage (Used & free memory)
    // 2. Performance information
    //      a. Duration of system uptime
    //      b. Duration of system idletime
    //          Uptime - Idletime = Busytime
    //          Busytime / Uptime = System utilization (% of system doing work)

    // Variables used to store information
    ifstream input;
    string hostname;
    string version;
    float uptimeA, uptimeB, deltaUptime, idletimeA, idletimeB, deltaIdletime, busytime, utilization;
    int totalMemory, usedMemory, freeMemory;
    string temp;

    cout<<"CS 431 PROJECT 1 - RACHEL J. MORRIS"<<endl;
    cout<<" -- Part A: General information -- "<<endl;

    // Get hostname
    input.open( "/proc/sys/kernel/hostname" );
    input>>hostname;
    input.close();
    cout<<"Hostname: "<<hostname<<endl;

    // Get date & time
    time_t now;
    time( &now );
    cout<<"System date and time: "<<ctime( &now );

    // Get kernel version
    input.open( "/proc/version" );
    input>>temp;    // "Linux"
    input>>temp;    // "version"
    input>>version;
    input.close();
    cout<<"Version: "<<version<<endl;

    // Get memory usage
    input.open( "/proc/meminfo" );
    input>>temp;   // "MemTotal:"
    input>>totalMemory;
    input>>temp;    // "kB"
    input>>temp;    // "MemFree: "
    input>>freeMemory;
    usedMemory = totalMemory - freeMemory;
    input.close();
    cout<<"Total memory: "<<totalMemory<<" kB"<<endl;
    cout<<"Free memory: "<<freeMemory<<" kB"<<endl;
    cout<<"Used memory: "<<usedMemory<<" kB"<<endl;

    cout<<"\n -- Part B: Performance information -- "<<endl;
    input.open( "/proc/uptime" );
    input>>uptimeA;
    input>>idletimeA;
    input.close();

    cout<<"Initial values: "<<endl;
    cout<<"Uptime: "<<uptimeA<<endl;
    cout<<"Idletime: "<<idletimeA<<endl;

    cout<<"\nBeginning 'busywork' function: "<<endl;

    Work( "BusyworkFunc" );

    input.open( "/proc/uptime" );
    input>>uptimeB;
    input>>idletimeB;
    input.close();

    deltaUptime = uptimeB - uptimeA;
    deltaIdletime = idletimeB - idletimeA;
    busytime = deltaUptime - deltaIdletime;
    utilization = busytime / deltaUptime;

    cout<<"\nFinal values: "<<endl;
    cout<<"Uptime: "<<uptimeB<<endl;
    cout<<"Idletime: "<<idletimeB<<endl;
    cout<<"Delta-Uptime: "<<deltaUptime<<endl;
    cout<<"Delta-Idletime: "<<deltaIdletime<<endl;
    cout<<"Busytime: "<<busytime<<endl;
    cout<<"System Utilization: "<<utilization<<endl;

    return 0;
}

void Work( char *name )
{
    // Cotter's work function, but changed i maximum from 5 to 500
    double y;
    double x = 3.0;
    double e = 2.0;
    int i, j;
    for ( i=0; i<500; i++ )
    {
        for ( j=0; j<1000000; j++ )
        {
            y = pow( x, e );
        }
        printf( "Loop %d of %s work cycle\n", i, name );
    }
}
