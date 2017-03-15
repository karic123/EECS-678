#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

struct thread_args
{
    int tid;
    int inc;
    int loop;
};

int count = 0;
pthread_mutex_t count_mutex;

void * inc_count( void * arg );

int main()
{
    int i, loop, inc;
    struct thread_args * targs;
    pthread_attr_t attr;
    pthread_t threads[NUM_THREADS];

    int argc = 3;
    char* argv[3] = { ".", "100000", "1" };

    if ( argc != 3 )
    {
        printf( "Usage: PTCOUNT LOOP_BOUND INCREMENT \n" );
        return 0;
    }

    loop    = atoi( argv[1] );
    inc     = atoi( argv[2] );

    pthread_mutex_init( &count_mutex, NULL );

    pthread_attr_init( &attr );
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

    for ( i = 0; i < NUM_THREADS; i++ )
    {
        targs = malloc( sizeof( targs ) );
        targs->tid = i;
        targs->loop = loop;
        targs->inc = inc;
        pthread_create( &threads[i], &attr, inc_count, (void*)targs );
    }

    for ( i = 0; i < NUM_THREADS; i++ )
    {
        pthread_join( threads[i], NULL );
    }


    return 0;
}

void * inc_count( void * arg )
{
    int i, loc;
    struct thread_args * my_args = (struct thread_args*) arg;

    loc = 0;
    for ( i = 0; i < my_args->loop; i++ )
    {
        __atomic_add_fetch( &count, my_args->inc, __ATOMIC_RELAXED );
        loc = loc + my_args->inc;
    }

    printf("Thread: %d finished. Counted: %d\n", my_args->tid, loc);
    free( my_args );
    pthread_exit( NULL );
}
