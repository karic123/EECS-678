/** @file libscheduler.h
 */

#ifndef LIBSCHEDULER_H_
#define LIBSCHEDULER_H_

#include "../libpriqueue/libpriqueue.h"

/**
  Constants which represent the different scheduling algorithms
*/
typedef enum {FCFS = 0, SJF, PSJF, PRI, PPRI, RR} scheme_t;


/* Structures ****************************************************
******************************************************************/

typedef struct _core_t
{
    bool processing;
    job_t* current_job;
} core_t;

typedef struct _scheduler_t
{
    // Statistics
    float waiting_time;
    float turn_around_time;
    float response_time;
    int jobs_finished;
    int last_recorded_time;

    // Scheduler scheme
    scheme_t sch;

    // Job queue
    priqueue_t queue;

    // Multi-core processing
    int core_count;
    core_t* core_array;
}scheduler_t;


/* Scheduling functions ******************************************
******************************************************************/

void  scheduler_start_up               (int cores, scheme_t scheme);
int   scheduler_new_job                (int job_number, int time, int running_time, int priority);
int   scheduler_job_finished           (int core_id, int job_number, int time);
int   scheduler_quantum_expired        (int core_id, int time);
float scheduler_average_turnaround_time();
float scheduler_average_waiting_time   ();
float scheduler_average_response_time  ();
void  scheduler_clean_up               ();

void  scheduler_show_queue             ();
void  scheduler_show_cores             ();
char* scheduler_scheme_name( scheme_t scheme );
void scheduler_update_metrics( int current_time );

int scheduler_locate_job_for_core( int core_id );
struct job_t* scheduler_get_next_job(int time);

int scheduler_locate_core_job_lowest_priority( int threshold, struct _job_t* ptrCurrent );
int scheduler_locate_core_job_highest_remaining( int threshold, struct _job_t* ptrCurrent );

int comparer_fcfs(const void* x,const void* y);
int comparer_sjf( const void* a, const void* b );
int comparer_psjf( const void* a, const void* b );
int comparer_pri( const void* a, const void* b );
int comparer_ppri( const void* a, const void* b );
int comparer_rr( const void* a, const void* b );

/************************************************
*   FCFS    - First Come First Serve            *
*   RR      - Round Robin, FCFS with preemption *
*                                               *
*   SJF     - Shortest Job First                *
*   PSJF    - SJF with preemption               *
*                                               *
*   PRI     - Priority-based scheduling scheme  *
*   PPRI    - PRI with preemption               *
*************************************************/

#endif /* LIBSCHEDULER_H_ */

