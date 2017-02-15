#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

// Callback functions for signals
void HandleSignalInterrupt();
void HandleSignalStop();
void HandleSignalAlarm();

// Example "programs"
void How_SimpleSignals();
void How_Pause();
void How_Sigaction();
void How_Alarm();

#define QUIT 4

int main()
{
    int choice = 0;

    while ( choice != QUIT )
    {
        printf( "\n\n" );
        printf( "What do you want a demonstration of? \n" );
        printf( "1. Simple signals \n" );
        printf( "2. Pause \n" );
        printf( "3. Alarm \n" );
        printf( "4. Exit \n" );

        scanf( "%d", &choice );

        printf( "\n\n" );

        switch( choice )
        {
            case 1: How_SimpleSignals();    break;
            case 2: How_Pause();            break;
            case 3: How_Alarm();            break;

            case QUIT: printf( "Bye \n" );  break;
            default: printf( "Invalid choice \n" );
        }
    }

    return 0;
}





/****************************************** Example 1 */

void How_SimpleSignals()
{
    printf( "Signals set for CTRL+C and CTRL+Z. \n" );

    // Bind function call to signal
    signal( SIGINT, HandleSignalInterrupt );    // CTRL+C
    signal( SIGTSTP, HandleSignalStop );        // CTRL+Z

    // Don't exit
    while ( 1 ) ;
}

/****************************************** Example 2 */

void How_Pause()
{
    printf( "Pause program until signal is received... Use CTRL+C to return. \n" );

    // Bind function call to signal
    signal( SIGINT, HandleSignalInterrupt );    // CTRL+C

    pause();

    printf( "Back to program execution! \n\n" );
}

/****************************************** Example 3 */

void How_Alarm()
{
    // Unmask SIGALRM signal
    struct sigaction signalAction;
    signalAction.sa_handler = HandleSignalAlarm;
    signalAction.sa_flags = SA_RESETHAND;

    sigfillset( &signalAction.sa_mask );
    sigaction( SIGALRM, &signalAction, NULL );

    // Get mask
    sigset_t mask;
    sigprocmask( 0, NULL, &mask );

    // Unblock
    sigdelset( &mask, SIGALRM );

    // Doesn't work yet
    int seconds = 1;
    printf( "Set alarm for program... %i seconds... \n", seconds );

    // Bind function call to signal
    signal( SIGALRM, HandleSignalAlarm );

    // Set alarm
    alarm( seconds );
    sigsuspend( &mask );
}

/****************************************** Signal handlers */

void HandleSignalInterrupt()
{
    printf( "Got interrupt signal \n" );

    // Reset signal
    signal( SIGINT, HandleSignalInterrupt );
}

void HandleSignalStop()
{
    printf( "Got stop signal \n" );

    // Reset signal
    signal( SIGINT, HandleSignalStop );
}

void HandleSignalAlarm()
{
    printf( "Got alarm signal \n" );

    // Reset signal
    signal( SIGALRM, HandleSignalAlarm );
}
