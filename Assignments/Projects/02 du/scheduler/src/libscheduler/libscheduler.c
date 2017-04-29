/** @file libscheduler.c
 */

/************************
*                       *
*       C++ > C!        *
*                       *
* I miss my OOP designs *
*                       *
*************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"

/* Named constants ***********************************************
******************************************************************/
const int NO_JOB = -1;
const int NO_CORE = -1;


/* Jen, tiu chi estas la taskplanilo. ****************************
 * If anyone else puts Esperanto in their code *******************
 * then they probably plagarized from me. ************************
******************************************************************/
scheduler_t schedulerObj;


/* Memory tracking ***********************************************
******************************************************************/

int totalMallocs = 0;
int totalFrees = 0;

struct _job_t* MallocJob( size_t size )
{
    struct job_t* ptr;
    ptr = malloc( size );
    totalMallocs++;
    DOutLbl( "MallocJob", "Total mallocs", totalMallocs );
    DOutLbl( "MallocJob", "Total frees", totalFrees );
    return ptr;
}

struct _core_t* MallocCore( size_t size )
{
    struct core_t* ptr;
    ptr = malloc( size );
    totalMallocs++;
    DOutLbl( "MallocCore", "Total mallocs", totalMallocs );
    DOutLbl( "MallocCore", "Total frees", totalFrees );
    return ptr;
}

void FreeJob( struct _job_t* ptr )
{
    totalFrees++;
    DOutLbl( "FreeJob", "Total mallocs", totalMallocs );
    DOutLbl( "FreeJob", "Total frees", totalFrees );
}

void FreeCore( struct _core_t* ptr )
{
    totalFrees++;
    DOutLbl( "FreeCore", "Total mallocs", totalMallocs );
    DOutLbl( "FreeCore", "Total frees", totalFrees );
}

/* Main Dooblydoos ***********************************************
******************************************************************/

/**
  Initalizes the scheduler.

  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/
void scheduler_start_up(int cores, scheme_t scheme)
{
    DOutFunc( "scheduler_start_up", true );

    // Initialize scheduler statistics
    schedulerObj.waiting_time=0;
    schedulerObj.turn_around_time=0;
    schedulerObj.response_time=0;
    schedulerObj.jobs_finished=0;
    schedulerObj.last_recorded_time = 0;

    // Create our array of cores
    schedulerObj.core_count = cores;

//    void MallocJob( struct job_t* ptr, size_t size )
    schedulerObj.core_array = MallocCore( sizeof( core_t ) * schedulerObj.core_count );
//    schedulerObj.core_array = malloc( sizeof( core_t ) * schedulerObj.core_count );     // MEM_ALLOC

    // Make sure to initialize job ptrs to NULL
    // since they're not allocated.
    for ( int i = 0; i < schedulerObj.core_count; i++ )
    {
        DOut( "", "Hi" );
        schedulerObj.core_array[i].current_job = NULL;
        schedulerObj.core_array[i].processing = false;
    }

    // Set up the comparer functions
    schedulerObj.sch = scheme;
    DOut( "scheduler_start_up", "Our scheme is" );
    DOut( "scheduler_start_up", scheduler_scheme_name( schedulerObj.sch ) );
    switch(schedulerObj.sch)
    {
        case FCFS:
            priqueue_init(&(schedulerObj.queue), comparer_fcfs);
        break;

        case SJF:
            priqueue_init(&(schedulerObj.queue), comparer_sjf);
        break;

        case PSJF:
            priqueue_init(&(schedulerObj.queue), comparer_psjf);
        break;

        case PRI:
            priqueue_init(&(schedulerObj.queue), comparer_pri);
        break;

        case PPRI:
            priqueue_init(&(schedulerObj.queue), comparer_ppri);
        break;

        case RR:
            priqueue_init(&(schedulerObj.queue), comparer_rr);
        break;

        default:
            printf( "Error: Scheme is unrecognized. Default to FCFS. \n" );
            priqueue_init(&(schedulerObj.queue), comparer_fcfs);
    }

    DOutFunc( "scheduler_start_up", false );
}


/**
  Called when a new job arrives.

  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.

  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on.

  If another job is already running on the core specified,
  this will preempt the currently running job.

  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made.

 */
int scheduler_new_job( int job_number, int time, int running_time, int priority )
{
    DOutFunc( "scheduler_new_job", true );

    scheduler_update_metrics( time );

    // Create a new job
//    job_t * new_job = malloc(sizeof(job_t));     // MEM_ALLOC
//    void MallocJob( struct job_t* ptr, size_t size )
    job_t* new_job = MallocJob( sizeof( job_t ) );

    new_job->job_id = job_number;

    new_job->arrival_time = time;
    new_job->arr_time_rr= time;
    new_job->running_time = running_time;
    new_job->remaining_time = running_time;
    new_job->turnaround_time = 0;

    new_job->priority = priority;
    new_job->core = NO_CORE;
    new_job->wait_time = 0;
    new_job->processing_time = 0;
    new_job->job_accp = false;
    new_job->response_time = 0 ;

    priqueue_offer( &schedulerObj.queue, new_job );

    // Find first available core, and how many cores are idle
    int availableCore = NO_CORE;
    int availableCoreCount = 0;
    for ( int i = 0; i < schedulerObj.core_count; i++ )
    {
        if ( availableCore == NO_CORE && schedulerObj.core_array[i].processing == false )
        {
            availableCore = i;
            availableCoreCount++;
        }
    }

    DOutLbl( "scheduler_new_job", "Available cores", availableCoreCount );
    DOutLbl( "scheduler_new_job", "Total cores", schedulerObj.core_count );
    DOutLbl( "scheduler_new_job", "Available core", availableCore );

    // If no cores are available currently, look for something
    // to preempt.
    bool swapJobs = false;
    if ( availableCore != NO_CORE )
    {
        DOut( "scheduler_new_job", "Assign new job to first available core" );
    }

    else if ( schedulerObj.sch == PSJF )
    {
        // Find out whether any cores have a higher-time-remaining job
        DOut( "scheduler_new_job", "PSJF scheduling" );
        availableCore = scheduler_locate_core_job_highest_remaining( new_job->remaining_time, new_job );

        if ( availableCore != NO_CORE )
        {

            swapJobs = true;
            DOut( "scheduler_new_job", "Preempt job with higher remaining time." );
            DOutLbl( "scheduler_new_job", "Current job remaining time", schedulerObj.core_array[ availableCore ].current_job->remaining_time );
            DOutLbl( "scheduler_new_job", "New     job remaining time", new_job->remaining_time );
        }
    }
    else if ( schedulerObj.sch == PPRI )
    {
        // Find out whether any cores have a lower-priority job
        DOut( "scheduler_new_job", "PPRI scheduling" );
        availableCore = scheduler_locate_core_job_lowest_priority( new_job->priority, new_job );

        if ( availableCore != NO_CORE )
        {
            swapJobs = true;
            DOut( "scheduler_new_job", "Preempt job with lower priority." );
        }
    }

    if ( availableCore != NO_CORE )
    {
        if ( swapJobs )
        {
            // Swap out job currently on that core
            // Get the current-running job
            job_t* ptrCurrentJob = schedulerObj.core_array[ availableCore ].current_job;
            // Replace with new job
            schedulerObj.core_array[ availableCore ].current_job = new_job;

            ptrCurrentJob->core = NO_CORE;
        }
        new_job->response_time = time - new_job->arrival_time;
	    new_job->job_accp = true;
        // Assign this job to the first available core
        schedulerObj.core_array[ availableCore ].current_job = new_job;
        schedulerObj.core_array[ availableCore ].processing = true;
        new_job->core = availableCore;


        DOutFunc( "scheduler_new_job", false );
        return availableCore;
    }


    // Otherwise, if we don't use preemptive scheduling,
    // or we have no job to preempt then it sits in the queue.

    DOutFunc( "scheduler_new_job", false );

    return NO_CORE;
}


/**
  Called when a job has completed execution.

  The core_id, job_number and time parameters are provided for convenience.
  You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.

  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time)
{
    DOutFunc( "scheduler_job_finished", true );

    DOutLbl( "scheduler_job_finished", "Job that finished is", job_number );

    scheduler_update_metrics( time );

    schedulerObj.jobs_finished++;
    job_t* ptrCurrent = schedulerObj.core_array[ core_id ].current_job;

    schedulerObj.waiting_time =schedulerObj.waiting_time + (time - ptrCurrent->arrival_time - ptrCurrent->running_time);
    schedulerObj.turn_around_time = schedulerObj.turn_around_time + (time - ptrCurrent->arrival_time);

    DOut( "scheduler_job_finished", "Remove the current job from the queue." );
    priqueue_remove( &schedulerObj.queue, ptrCurrent );


    schedulerObj.core_array[ core_id ].current_job = NULL;
    schedulerObj.core_array[ core_id ].processing = false;
    schedulerObj.response_time = schedulerObj.response_time+ptrCurrent->response_time;


    FreeJob( ptrCurrent );
    DOut( "Hi", "Bye" );
    ptrCurrent = NULL;
//    free( ptrCurrent );

	job_t* ptrNextJob = scheduler_get_next_job(time);
	if ( ptrNextJob == NULL )
	{
        DOutLbl( "scheduler_job_finished", "No job available to give to core", core_id );
        return -1;
	}
	else
	{
        DOutLbl( "scheduler_job_finished", "Assign new job to core", core_id );
        DOutLbl( "scheduler_job_finished", "Assign job ", ptrNextJob->job_id );
        schedulerObj.core_array[ core_id ].current_job = ptrNextJob;
        schedulerObj.core_array[ core_id ].processing = true;
        ptrNextJob->core = core_id;
        return ptrNextJob->job_id;
	}
}

/*
    Look through all the cores and get the one with the lowest
    priority below the treshold. If nothing meets the criteria,
    return -1.
*/
int scheduler_locate_core_job_lowest_priority( int threshold, struct _job_t* ptrCurrent )
{
    DOut( "scheduler_locate_core_job_lowest_priority", "PPRI scheduling" );

    int preemptIndex = -1;
    int preemptThreshold = -1;
    int preemptArrivalThreshold = -1;

    DOutLbl( "scheduler_locate_core_job_lowest_priority", "New job id", ptrCurrent->job_id );
    DOutLbl( "scheduler_locate_core_job_lowest_priority", "New job priority", threshold );
    DOutLbl( "scheduler_locate_core_job_lowest_priority", "New job arrival ", ptrCurrent->arrival_time );
    DOutLbl( "scheduler_locate_core_job_lowest_priority", "New job runtime ", ptrCurrent->running_time );

    for ( int i = 0; i < schedulerObj.core_count; i++ )
    {
        DOut( "", ":|" );
        DOutLbl( "scheduler_locate_core_job_lowest_priority", "Look at job on core ", i );

        if ( schedulerObj.core_array[i].current_job != NULL )
        {
            DOutLbl( "scheduler_locate_core_job_lowest_priority", "Job ID   ", schedulerObj.core_array[i].current_job->job_id );
            DOutLbl( "scheduler_locate_core_job_lowest_priority", "Priority ", schedulerObj.core_array[i].current_job->priority );
            DOutLbl( "scheduler_locate_core_job_lowest_priority", "Arrival  ", schedulerObj.core_array[i].current_job->arrival_time );
            DOutLbl( "scheduler_locate_core_job_lowest_priority", "Runtime  ", schedulerObj.core_array[i].current_job->running_time );
        }


        // First: See if our new job could preempt this core's current job
        // Is the new job a higher priority (that is, a LOWER NUMBER PRIORITY)
        // than the current running item?
        if ( schedulerObj.core_array[i].current_job != NULL
            &&
            comparer_ppri( schedulerObj.core_array[i].current_job, ptrCurrent ) >= 1 )
//            schedulerObj.core_array[i].current_job->priority > threshold )
        {
            DOutLbl( "scheduler_locate_core_job_lowest_priority", "This job is lower-priority than new job. But is it lowest? (Highest #)...", schedulerObj.core_array[i].current_job->priority );

            DOutLbl( "scheduler_locate_core_job_lowest_priority", "Current HIGHEST is ", preemptThreshold );
            // But is this the best core to preempt?
            // Compare it to the preemptThreshold, which holds the
            // highest numbered priority that we've found. If the current core's
            // priority is HIGHER NUMBERED, this is our new max.
            // Update the index and threshold.

            DOutLbl( "", "Current job priority", schedulerObj.core_array[i].current_job->priority );
            DOutLbl( "", "Largest job priority", preemptThreshold );
            if ( preemptThreshold == -1
                ||
                comparer_ppri( schedulerObj.core_array[i].current_job, ptrCurrent ) > 1 )
//                schedulerObj.core_array[i].current_job->priority > preemptThreshold )
            {
                preemptIndex = i;
                preemptThreshold = schedulerObj.core_array[i].current_job->priority;
                preemptArrivalThreshold = schedulerObj.core_array[i].current_job->arrival_time;
                DOutLbl( "scheduler_locate_core_job_lowest_priority", "Set new HIGHEST priority Treshold to ", preemptThreshold );
                DOutLbl( "scheduler_locate_core_job_lowest_priority", "Set new HIGHEST arrival  Treshold to ", preemptArrivalThreshold );
                DOutLbl( "scheduler_locate_core_job_lowest_priority", "Set new HIGHEST at core     ", i );
            }
            else if ( schedulerObj.core_array[i].current_job->priority == preemptThreshold
                && schedulerObj.core_array[i].current_job->arrival_time >= preemptArrivalThreshold
                )
            {
                DOut( "scheduler_locate_core_job_lowest_priority", "Equal priorities" );
                preemptIndex = i;
                preemptThreshold = schedulerObj.core_array[i].current_job->priority;
                preemptArrivalThreshold = schedulerObj.core_array[i].current_job->arrival_time;
                DOutLbl( "scheduler_locate_core_job_lowest_priority", "Set new HIGHEST priority Treshold to ", preemptThreshold );
                DOutLbl( "scheduler_locate_core_job_lowest_priority", "Set new HIGHEST arrival  Treshold to ", preemptArrivalThreshold );
                DOutLbl( "scheduler_locate_core_job_lowest_priority", "Set new HIGHEST at core     ", i );
            }
        }
    }

    DOutLbl( "scheduler_locate_core_job_lowest_priority", "Preempt Index", preemptIndex );
    return preemptIndex;
}

/*
    Look through all the cores and get the one with the highest
    remaining time above the threshold. If nothing meets the
    criteria, return -1.
*/
int scheduler_locate_core_job_highest_remaining( int threshold, struct _job_t* ptrCurrent )
{
    DOut( "scheduler_locate_core_job_highest_remaining", "PSJF scheduling" );

    int preemptIndex = -1;
    int preemptTime = -1;

    DOutLbl( "scheduler_locate_core_job_highest_remaining", "New job remaining time", threshold );

    for ( int i = 0; i < schedulerObj.core_count; i++ )
    {
        DOutLbl( "scheduler_locate_core_job_highest_remaining", "Look at job ", i );

        if ( schedulerObj.core_array[i].current_job != NULL )
        {
            DOutLbl( "scheduler_locate_core_job_highest_remaining", "Remaining time ", schedulerObj.core_array[i].current_job->remaining_time );
        }

        // First: See if our new job could preempt this core's current job
        // Does the new job have a LOWER REMAINING TIME than the current
        // core job?
        if ( schedulerObj.core_array[i].current_job != NULL
            &&
            comparer_psjf( schedulerObj.core_array[i].current_job, ptrCurrent ) >= 1 )
//            && schedulerObj.core_array[i].current_job->remaining_time > threshold )
        {
            // But is this the best core to preempt?
            // We need to find the largest remaining time to replace.
            // Check this core's job's remaining time vs. the last "max" we found.
            // TODO: double-check logic
            if ( preemptTime == -1
                ||
//                comparer_psjf( schedulerObj.core_array[i].current_job, ptrCurrent ) > 1 )
            schedulerObj.core_array[i].current_job->remaining_time > preemptTime )
            {
                preemptIndex = i;
                preemptTime = schedulerObj.core_array[i].current_job->remaining_time;
            }
        }
    }

    DOutLbl( "scheduler_locate_core_job_highest_remaining", "Preempt Index", preemptIndex );
    return preemptIndex;
}


struct job_t* scheduler_get_next_job(int time)
{
    DOutFunc( "scheduler_get_next_job", true );
    DOutLbl( "scheduler_get_next_job", "Queue size", priqueue_size( &schedulerObj.queue ) );

    // Find a job that isn't currently on a core.
    job_t* ptrCurrent = NULL;
    int queueSize = priqueue_size( &schedulerObj.queue );
    for ( int i = 0; i <= queueSize; i++ ) // allow it to go outside the queue bounds to NULL.
    {
        ptrCurrent = (job_t*)(priqueue_at( &schedulerObj.queue, i ));

        if ( ptrCurrent != NULL && ptrCurrent->core == NO_CORE )
        {
            break;
        }
    }

    DOutLbl( "scheduler_get_next_job", "Job address", ptrCurrent );

    if ( ptrCurrent == NULL )
    {
        DOut( "scheduler_get_next_job", "No job found" );
    }
    else
    {
        DOutLbl( "scheduler_get_next_job", "Return job", ptrCurrent->job_id );
        DOutLbl( "scheduler_get_next_job", "Job core", ptrCurrent->core );
    }

    if (ptrCurrent != NULL && ptrCurrent->job_accp == false)
	{
	ptrCurrent->response_time = time - ptrCurrent->arrival_time;
	ptrCurrent->job_accp = true;
	}
    // Will return NULL or the job...
    return ptrCurrent;
}

/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.

  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time)
{
    DOutFunc( "scheduler_quantum_expired", true );
    DOutLbl( "scheduler_quantum_expired", "Queue size", priqueue_size( &schedulerObj.queue ) );

    //scheduler_update_metrics( time );

    // Throw the current job back into the queue,
    // and it will be re-run later.
    job_t* preemptedJob = schedulerObj.core_array[ core_id ].current_job;

    DOutLbl( "scheduler_quantum_expired", "Preempted job is", preemptedJob->job_id );

    // Update arrival time? So that it goes to the end of the queue.
    preemptedJob->arr_time_rr = time;
    preemptedJob->core = NO_CORE; // tell it that it isn't processing anymore

    DOutLbl( "scheduler_quantum_expired", "Remove preempted job from queue and re-insert", preemptedJob->job_id );

    priqueue_remove( &schedulerObj.queue, preemptedJob ); // remove from queue
    priqueue_offer( &schedulerObj.queue, preemptedJob ); // put at end

    schedulerObj.core_array[ core_id ].current_job = NULL;

    job_t* nextJob = scheduler_get_next_job(time); // get next item in queue

    if ( nextJob != NULL )
    {
        DOut( "scheduler_quantum_expired", "Pulled next available item from queue." );
        DOutLbl( "scheduler_quantum_expired", "Next job is", nextJob->job_id );

        schedulerObj.core_array[ core_id ].current_job = nextJob;
        nextJob->core = core_id;
        return nextJob->job_id;
    }

    DOutFunc( "scheduler_quantum_expired", false );
	return NO_JOB;
}

/**
  Free any memory associated with your scheduler.

  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{
    // TODO: Double check anything that is allocating memory.

    DOutFunc( "scheduler_clean_up", true );

    // free data within node first...
    for ( int i = 0; i < priqueue_size( &schedulerObj.queue ); i++ )
    {
        struct job_t* data = priqueue_at( &schedulerObj.queue, i );
        if ( data != NULL )
        {
            FreeJob( data );
//            free( data );
            data = NULL;
        }
    }

    priqueue_destroy( &schedulerObj.queue );
    FreeCore( schedulerObj.core_array );
//    free( schedulerObj.core_array );

    DOutLbl( "scheduler_clean_up", "TOTAL MALLOCS ", totalMallocs );
    DOutLbl( "scheduler_clean_up", "TOTAL FREES   ", totalFrees );

    DOutFunc( "scheduler_clean_up", false );
}


/* Metrics stuff *************************************************
******************************************************************/

void scheduler_update_metrics( int current_time )
{
    DOutFunc( "scheduler_update_metrics", true );

    // last_recorded_time
    int delta = current_time - schedulerObj.last_recorded_time;

    DOutLbl( "scheduler_update_metrics", "Current timestamp       ", current_time );
    DOutLbl( "scheduler_update_metrics", "Last recorded timestamp ", schedulerObj.last_recorded_time );
    DOutLbl( "scheduler_update_metrics", "Time delta              ", delta );

    schedulerObj.last_recorded_time = current_time;

    // Add to wait time - queue = waiting jobs
    int queueSize = priqueue_size( &schedulerObj.queue );
    DOutLbl( "scheduler_update_metrics", "Queue size", queueSize );
    for ( int i = 0; i < queueSize; i++ )
    {
        job_t* ptrJob = (job_t*)(priqueue_at( &schedulerObj.queue, i ) );
        if ( ptrJob != NULL && ptrJob->core == NO_CORE )
        {
            ptrJob->wait_time += delta;
            ptrJob->total_time += delta;
        }
    }

    // Add to processing time - in cores = processing jobs
    // Remove from remaining time?
    DOutLbl( "scheduler_update_metrics", "Core count", schedulerObj.core_count );
    for ( int i = 0; i < schedulerObj.core_count; i++ )
    {
        if ( schedulerObj.core_array[i].current_job != NULL )
        {
            schedulerObj.core_array[i].current_job->processing_time += delta;
            schedulerObj.core_array[i].current_job->total_time += delta;
            schedulerObj.core_array[i].current_job->remaining_time -= delta;
        }
    }

    DOutFunc( "scheduler_update_metrics", false );
}

/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
	if(schedulerObj.jobs_finished == 0)
	{
		return 0.0;
	}
	else
	{
		float avg_wait_time = (schedulerObj.waiting_time)/(schedulerObj.jobs_finished);
		return avg_wait_time;
	}
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
if(schedulerObj.jobs_finished == 0)
    {
        return 0.0;
    }
    else
    {
        float avg_res_time = (schedulerObj.response_time)/(schedulerObj.jobs_finished);
        return avg_res_time;
    }
//return -1;
}

/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
	if(schedulerObj.jobs_finished == 0)
	{
		return 0.0;
	}
	else
	{
		float avg_turnaround_time = (schedulerObj.turn_around_time)/(schedulerObj.jobs_finished);
		return avg_turnaround_time;
	}
}


/* Display stuff *************************************************
******************************************************************/

/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)

  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{
    DOutFunc( "scheduler_show_queue", true );

    printf( "\n QUEUE: \n" );
    priqueue_display( &(schedulerObj.queue) );

    DOutFunc( "scheduler_show_queue", false );

    scheduler_show_cores();
}

void scheduler_show_cores()
{
    DOutFunc( "scheduler_show_cores", true );
    printf( "\n CORES: \n" );

    for ( int i = 0; i < schedulerObj.core_count; i++ )
    {
        if ( schedulerObj.core_array[i].current_job != NULL )
        {
            printf( "Core item %d  \t", i );
            printf( "\t Running Job ID:   %d ", schedulerObj.core_array[i].current_job->job_id );
            printf( "\t Arrival time:     %d ", schedulerObj.core_array[i].current_job->arrival_time );
            printf( "\t Job Priority:     %d \n", schedulerObj.core_array[i].current_job->priority );
        }
    }

    DOutFunc( "scheduler_show_cores", false );
}



/* Debuggo *******************************************************
******************************************************************/

char* scheduler_scheme_name( scheme_t scheme )
{
    if      ( scheme == FCFS )
    {
        return "FCFS";
    }
    else if ( scheme == SJF )
    {
        return "SJF";
    }
    else if ( scheme == PSJF )
    {
        return "PSJF";
    }
    else if ( scheme == PRI )
    {
        return "PRI";
    }
    else if ( scheme == PPRI )
    {
        return "PPRI";
    }
    else if ( scheme == RR )
    {
        return "RR";
    }
    else
    {
        return "UNKNOWN";
    }
}


/* Comparer functions ********************************************
******************************************************************/

int comparer_fcfs(const void* a,const void* b)//fcfs places next element at the end of queue
{
    job_t jobA = *((job_t*)a);
    job_t jobB = *((job_t*)b);

    if      ( jobA.arrival_time < jobB.arrival_time ) { return -1; }
    else if ( jobA.arrival_time > jobB.arrival_time ) { return 1; }
    else { return 0; }
}

int comparer_sjf( const void* a, const void* b )
{
//sjf function sorts elements based on running time left for the job and if the running time for the two jobs is same then sorts it according to arrival time
    job_t jobA = *((job_t*)a);
    job_t jobB = *((job_t*)b);
    if(jobA.running_time < jobB.running_time)
        return -1;
    else if(jobA.running_time > jobB.running_time)
        return 1;
    else
        return (jobA.arrival_time -jobB.arrival_time);
}

int comparer_psjf( const void* a, const void* b )
{
    // Preemptive Shortest Job First:pre-empts jobs based on the shortest remaining time
    job_t jobA = *((job_t*)a);
    job_t jobB = *((job_t*)b);
    return (jobA.remaining_time - jobB.remaining_time);
}

//this function compares the wo jobs based on their priorities
int comparer_pri( const void* a, const void* b )
{
    //PRI function compares the two jobs based on their priorities
    job_t jobA = *((job_t*)a);
    job_t jobB = *((job_t*)b);

    if(jobA.priority > jobB.priority)
    {
        return 1;
    }
    else if(jobA.priority < jobB.priority)
    {
        return -1;
    }
    else
    {
        return(jobA.arrival_time - jobB.arrival_time);
    }
}

int comparer_ppri( const void* a, const void* b )
{
  //PPRI functions always runs the most important job available,pre empting currently running job
    job_t jobA = *((job_t*)a);
    job_t jobB = *((job_t*)b);

    if(jobA.priority == jobB.priority)
    {
        // Later arrival = less priority
        if ( jobA.arrival_time > jobB.arrival_time )
        {
            return 1;
        }
        else if ( jobA.arrival_time < jobB.arrival_time )
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return (jobA.priority -jobB.priority);
    }
}

// Round Robin scheduling comparer function
int comparer_rr( const void* a, const void* b )
{
// Round Robin scheduling comparer function:run each job for a specific time quantum.
    job_t jobA = *((job_t*)a);
    job_t jobB = *((job_t*)b);

    if(jobA.arr_time_rr > jobB.arr_time_rr)
    {
        return 1;
    }
    else if(jobA.arr_time_rr < jobB.arr_time_rr)
    {
        return -1;
    }

    return 0;
}



/* Rachel was here ***********************************************
******************************************************************/

/*
     ^----^     <( Meow )
   =|o   o|=
     --`--___
     |   |  _\
     |___|_/
*/

/* I want to play video games. */
