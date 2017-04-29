/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_

#include <stdbool.h>


void DOut( char* functionName, char* message );
void DOutLbl( char* functionName, char* message, int value );
void DOutFunc( char* functionName, bool functionBegin );

typedef struct _job_t
{
    int job_id; //job id
    int arrival_time; //job arrival time
    int arr_time_rr;
    int running_time;  //job run time
    int remaining_time; //remaining time of job
    int priority; //Priority of Job
    //int first_run_time; //core initial run time
    //int upt_core_time; //core last updated time
    int time;
    int hd; // head
    int jobs;
    int core;
    int wait_time;
    int processing_time;
    int total_time;
    int turnaround_time;
    int response_time;
    bool job_accp;
} job_t;


/**
    Node, dangit.
*/
struct node_t
{
    void * data;
    struct node_t * ptrNext;
    struct node_t * ptrPrev;
};

/**
  Priqueue Data Structure
*/
typedef struct _priqueue_t
{
    struct node_t * ptrHead;
    struct node_t * ptrTail;
    int size;
    int (*compareFunction)(const void*, const void*);

} priqueue_t;

void   priqueue_init     (priqueue_t *q, int(*comparer)(const void *, const void *));

void   priqueue_push_back(priqueue_t *q, void *ptr);
int    priqueue_offer    (priqueue_t *q, void *ptr);
void * priqueue_peek     (priqueue_t *q);
void * priqueue_poll     (priqueue_t *q);
void * priqueue_at       (priqueue_t *q, int index);
int    priqueue_remove   (priqueue_t *q, void *ptr);
void * priqueue_remove_at(priqueue_t *q, int index);
int    priqueue_size     (priqueue_t *q);

void   priqueue_destroy  (priqueue_t *q);

void   priqueue_display  ( priqueue_t *q );

#endif /* LIBPQUEUE_H_ */
