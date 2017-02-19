/* Rachel J Morris, EECS 678, Lab 4, 2017 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>

int terminalStopCounter = 0;
const int QUIT_INTERRUPT_THRESHOLD = 5;
const int QUIT_TIMER_THRESHOLD = 10;
bool idleFlag = false;

void HandleInterrupt( int signal );
void HandleTerminalStop( int signal );
void HandleAlarm();
void SetupInterruptSignals();
void SetupAlarmSignal();

int main()
{
    SetupInterruptSignals();

    while ( 1 );

    return 0;
}

void SetupInterruptSignals()
{
    signal( SIGTSTP, HandleTerminalStop );
    signal( SIGINT, HandleInterrupt );
}

void HandleTerminalStop( int signal )
{
    printf( "\n\nSo far, '%d' Ctrl-C presses were counted\n\n", terminalStopCounter );
    fflush( stdout );
}

void HandleInterrupt( int signal )
{
    terminalStopCounter++;
    if ( terminalStopCounter >= QUIT_INTERRUPT_THRESHOLD )
    {
        // Set up timer that will "log out" after 10 seconds
        SetupAlarmSignal();
        idleFlag = true;

        // Prompt user whether they want to quit
        char answer[30];

        printf("\nReally exit? [Y/n]: ");
        fflush( stdout );
        fgets( answer, sizeof( answer ), stdin );

        if ( tolower( answer[0] ) == 'y' )
        {
            printf("\nExiting...\n");
            fflush( stdout );
            exit( 0 );
        }
        else
        {
            idleFlag = false;
            printf("\nContinuing\n");
            fflush( stdout );
            terminalStopCounter = 0;
        }
    }
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

void HandleAlarm()
{    
    if ( idleFlag )
    {
        fflush( stdout );
        // Alarm went off, user is idle.
        exit( 0 );
    }
    // Otherwise: don't quit
    fflush( stdout );
}
