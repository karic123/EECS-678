#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void CreateAndWait( int threadCount );
void CreateWithMutex( int threadCount );

int main()
{
    bool done = false;
    while ( !done )
    {
        printf( "0. Exit \n" );
        printf( "1. Create threads and wait \n" );
        printf( "2. Create threads and mutexes \n" );

        int choice;
        printf( ">> " );
        scanf( "%d", &choice );

        int threadCount;
        if ( choice == 1 || choice == 2 )
        {
            printf( "\n" );
            printf( "How many threads? \n" );

            printf( ">> " );
            scanf( "%d", &threadCount );
        }

        if ( choice == 0 )
        {
            done = true;
        }
        else if ( choice == 1 )
        {
            CreateAndWait( threadCount );
        }
        else if ( choice == 2 )
        {
            CreateWithMutex( threadCount );
        }

        printf( "\n\n" );
    }

    return 0;
}


pthread_t* threadList;

void ThreadFunction1( void* arg )
{
    int threadID = (int)arg;
    printf( "Running thread... %d \n", threadList[ threadID ] );
    pthread_exit( NULL );
}

void CreateAndWait( int threadCount )
{
    printf( "\n" );

    threadList = malloc( threadCount * sizeof( pthread_t ) ); //new pthread_t[ threadCount ];

    // Create threads
    printf( "Creating %d threads... \n", threadCount );
    for ( int i = 0; i < threadCount; i++ )
    {
        pthread_create( &threadList[i], NULL, ThreadFunction1, (void*)i );
    }

    // Wait for threads
    printf( "Waiting for %d threads... \n", threadCount );
    for ( int i = 0; i < threadCount; i++ )
    {
        pthread_join( threadList[i], NULL );
    }

    //delete [] threadList;
    free( threadList );
}

int counter = 0;
pthread_mutex_t counter_mutex;

void ThreadFunction2( void* arg )
{
    int threadID = (int)arg;

    int countDown = 5;

    while ( countDown > 0 )
    {
        pthread_mutex_lock( &counter_mutex );
        printf( "Thread %d modifies count to %d \n", threadList[ threadID ], counter );
        counter--;
        countDown--;
        pthread_mutex_unlock( &counter_mutex );
    }
    pthread_exit( NULL );
}

void CreateWithMutex( int threadCount )
{
    printf( "\n" );

    counter = 100;

    // Initialize mutex
    pthread_mutex_init( &counter_mutex, NULL );

    threadList = malloc( threadCount * sizeof( pthread_t ) );

    // Create threads
    printf( "Creating %d threads... \n", threadCount );
    for ( int i = 0; i < threadCount; i++ )
    {
        pthread_create( &threadList[i], NULL, ThreadFunction2, (void*)i );
    }

    // Wait for threads
    printf( "Waiting for %d threads... \n", threadCount );
    for ( int i = 0; i < threadCount; i++ )
    {
        pthread_join( threadList[i], NULL );
    }

    free( threadList );

    printf( "Counter is now %d. \n\n", counter );

    // Destroy mutex
    pthread_mutex_destroy( &counter_mutex );
}
