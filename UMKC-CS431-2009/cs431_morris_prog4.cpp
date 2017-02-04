/*
File name:      CS431_Morris_prog4.cpp
Author:         Rachel J. Morris
Due Date:       November 12, 2009

Focus:		Learning about the use of synchronization devices like semaphores and mutexes.

Description:	Write a lottery program where each number
		can be sold only once.  Therefore, we will
		only have one winner.  The range of lottery
		tickets is from 0 to 999,999, and you need to
		support at least 15 agents sellign tickets
		at the same time (multiple threads).

		You can assign numbers randomly or sequentially.
		Either way, you need to track (by vendor) how many
		tickets have been sold.  Once all 1,000,000 have
		been sold, close down the lottery.
		At the end, each agent must print out how many tickets
		they sold.

		Pick a winning ticket randomly and identify
		the agent that sold that ticket.

		When people come in to buy tickets, they will
		buy a random number of tickets from 1 to 10.

		Between each ticket sale, the clerk should do
		some work.
*/

#include <iostream>         // I/O
#include <pthread.h>        // POSIX multithreading
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

// g++ -pthread cs431_Morris_prog4.cpp

//#define ONE_MILLION 10000
#define ONE_MILLION 1000000
#define THREAD_AMT 15

void* SellTickets( void* arg );
void BusyWork();

int ticket[ONE_MILLION];
int ticketsSold[THREAD_AMT];
int totalTickets;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    cout<<"\nLOTTERY PROGRAM!"<<endl;
    cout<<"Rachel J. Morris, Nov 2009"<<endl<<endl;

    /* Set up variable.
        There will be 15 "Agents" selling tickets.

        Agents need to keep track of which tickets they sell.
        Tickets are between 0 to 1,000,000.

        I will keep an array of integers, with the
        subscript symbolizing the ticket amount, and
        the value of the subscript symbolizing which
        agent sold this ticket.*/

    pthread_t *thread = new pthread_t[THREAD_AMT];
    int winningTicket = -1;
    totalTickets = 0;
    bool sellingTickets = true;

    // Initialize values of arrays
    for ( int i=0; i<ONE_MILLION; i++ )
        ticket[i] = -1;
    for ( int i=0; i<THREAD_AMT; i++ )
    {
        // Initialize the count of how many tickets each agent sold
        ticketsSold[i] = 0;
    }

    srand( time(NULL) );

    /* Create threads */
    cout<<"Opening vendors... ";
    for ( int i=0; i<THREAD_AMT; i++ )
    {
        // pthread_create( pthread_t *thread,
        //      const pthread_attr_t *attributes,
        //      void *(*start_routine)(void*), void *arguments );
        pthread_create( &thread[i], NULL, SellTickets, (void*)i );
        cout<<i;
        if ( i != THREAD_AMT-1 )
            cout<<", ";
    }

    /* Main loop until all tickets are sold */
    cout<<"\nAll lotto ticket agents are now open and selling tickets."<<endl;

    while ( sellingTickets )
    {
        pthread_mutex_lock( &mutex );
        // not sure if it's really necessary to lock this region
        // since the if statement is just checking the variable and
        // not modifying it.

            if ( totalTickets >= ONE_MILLION-1 )
            {
                sellingTickets = false;
            }

        pthread_mutex_unlock( &mutex );
    }

    /* Done selling tickets, close (cancel) agents */
    cout<<"\nClosing vendors... ";
    for ( int i=0; i<THREAD_AMT; i++ )
    {
        cout<<i;
        if ( i != THREAD_AMT-1 )
            cout<<", ";

        pthread_cancel( thread[i] );
    }

    /* Get a winning number */
    winningTicket = rand() % ONE_MILLION + 1;

    /* Display statistics */
    cout<<"\nThe lotto is closed!  Here are the statistics!"<<endl;
    // How many tickets did each agent sell?
    cout<<"\n\tAGENT #\t\tTICKETS SOLD"<<endl;
    for ( int i=0; i<=THREAD_AMT; i++ )
    {
        if ( i < THREAD_AMT )
            cout<<"\t"<<i<<"\t\t"<<ticketsSold[i]<<endl;
        else
            cout<<"\tTotal:\t\t"<<totalTickets+1<<endl; // Tickets are from 0 to 999,999, so add one.
    }
    cout<<"\nThe winning ticket is ticket number "<<winningTicket<<"!!"<<endl;
    cout<<"\nThe winning ticket was sold by agent "<<ticket[winningTicket]<<"!"<<endl;

    /* Clean up memory before quitting */
    if ( thread != NULL )
    {
        delete [] thread;
        thread = NULL;
    }

    cout<<"\nEnd of program"<<endl;

	return 0;
}

/*
Sell Tickets
Input - Index of Agent
Output - Nothing returned
Description - Keeps thread in a while loop, selling tickets
                and then doing busy work until it is killed by main.
*/
void* SellTickets( void* arg )
{
    int myId = (int)arg;
    int amtOfTickets=0;
    int index = -1;

    while ( 1 )
    {
        // Sell a random amount of tickets (0 to 1) to a customer.
        // I am going to distribute tickets in sequential order.

        pthread_mutex_lock( &mutex ); // Lock down critical section
        if ( totalTickets < ONE_MILLION )
        {
            if ( ONE_MILLION - totalTickets <= 10 )
                amtOfTickets = rand() % (ONE_MILLION - totalTickets);
            else
                amtOfTickets = rand() % 10 + 1;

            for ( int i=0; i<amtOfTickets; i++ )
            {
                ticket[totalTickets] = myId;
                totalTickets++;
                ticketsSold[myId] += 1;
            }
        }
        pthread_mutex_unlock( &mutex ); // Free variables so others can modify.

        BusyWork();
    }
}

/*
Busywork
Input - none
Output - none
Description - Gives each thread some work to do between selling tickets.
*/
void BusyWork()
{
    double answer;

    for ( int workLoop = 0; workLoop < 9000; workLoop++ )
    {
        answer = pow(17.3, 9.2);
    }
}










