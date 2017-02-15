/* Rachel J Morris, EECS 678, Lab 4, 2017 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int terminalStopCounter = 0;
const int QUIT_INTERRUPT_THRESHOLD = 5;
const int QUIT_TIMER_THRESHOLD = 10;

void HandleInterrupt( int signal );
void HandleTerminalStop( int signal );
void HandleAlarm();
void SetupInterruptSignals();
void SetupAlarmSignal();

int main()
{
    SetupInterruptSignals();

    printf( "Welcome to program. \n " );
    printf( "Press CTRL+C 5 times to prompt for quit, " );
    printf( "or CTRL+Z to view tstp counter. \n\n" );
    fflush( stdout );

    while ( 1 );

    return 0;
}

void HandleInterrupt( int signal )
{
    terminalStopCounter++;
    if ( terminalStopCounter >= QUIT_INTERRUPT_THRESHOLD )
    {
        printf( "\n\n QUIT interrupt threshold hit! \n" );
        printf( "Wait %i seconds to quit, or enter 'y' to exit. \n\n", QUIT_TIMER_THRESHOLD );

        // Set up timer that will "log out" after 10 seconds
        SetupAlarmSignal();

        // Prompt user whether they want to quit
        char answer[30];
        printf( "\n Really exit? (y/n): " );
        fflush( stdout );
        fgets( answer, sizeof( answer ), stdin );

        if ( tolower( answer[0] ) == 'n' )
        {
            printf( "\n Continuing... \n" );
            fflush( stdout );
            // Reset counter
            terminalStopCounter = 0;
        }
        else
        {
            printf( "\n Exiting... \n" );
            fflush( stdout );
            exit( 0 );
        }
    }

    printf( "Interrupt \n" );
}

void HandleTerminalStop( int signal )
{
    printf( "\n\n So far, '%d' Ctrl-C presses were counted. \n\n", terminalStopCounter );
    fflush( stdout );
}

void HandleAlarm()
{
    printf( " TIMEOUT! Exiting. \n\n" );
}

void SetupInterruptSignals()
{
    signal( SIGTSTP, HandleTerminalStop );
    signal( SIGINT, HandleInterrupt );
}

void SetupAlarmSignal()
{
    // Set up HandleAlarm to be the function
    // to handle the signal
    struct sigaction signalAction;
    signalAction.sa_handler = HandleAlarm;
    sigaction( SIGALRM, &signalAction, NULL );

    // Get mask and unblock
    //sigfillset( &signalAction.sa_mask );

    sigset_t mask;
    sigprocmask( 0, NULL, &mask );
    sigdelset( &mask, SIGALRM );

    //signal( SIGALRM, HandleAlarm );
    alarm( QUIT_TIMER_THRESHOLD );
}
