/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"

const bool RACH_DEBUG = false;

void DOut( char* functionName, char* message )
{
    if ( RACH_DEBUG )
    {
        printf( "\t\t***** DEBUG [%s]: %s \n", functionName, message );
    }
}

void DOutLbl( char* functionName, char* message, int value )
{
    if ( RACH_DEBUG )
    {
        printf( "\t\t***** DEBUG [%s]: %s = %d \n", functionName, message, value );
    }
}

void DOutFunc( char* functionName, bool functionBegin )
{
    if ( RACH_DEBUG )
    {
        if ( functionBegin )
        {
            printf( "\n\t\t** DEBUG [%s]: FUNCTION BEGIN \n", functionName );
        }
        else
        {
            printf( "\n\t\t** DEBUG [%s]: FUNCTION END \n\n", functionName );
        }
    }
}

/**
  Initializes the priqueue_t data structure.

  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
    q->ptrHead = NULL;
    q->ptrTail = NULL;
    q->size = 0;
    q->compareFunction = comparer;
}


/**
  Inserts the specified element to the end of the queue, without prioritizing it.
  (This is mostly for my unit tests)

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
 */
void priqueue_push_back(priqueue_t *q, void *ptr)
{
    struct node_t* newNode = malloc( sizeof( struct node_t ) );
    newNode->data = ptr;
    newNode->ptrNext = newNode->ptrPrev = NULL; // initialize

    // Empty queue
    if ( q->size == 0 )
    {
        q->ptrHead = newNode;
        q->ptrTail = newNode;
        q->size++;
        return;
    }

    // Queue already has items
    q->ptrTail->ptrNext = newNode;
    newNode->ptrPrev = q->ptrTail;
    q->ptrTail = newNode;
    q->size++;
}

/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
    struct node_t* newNode = malloc( sizeof( struct node_t ) );
    newNode->data = ptr;
    newNode->ptrNext = newNode->ptrPrev = NULL; // initialize

    // Empty queue
    if ( q->size == 0 )
    {
        q->ptrHead = newNode;
        q->ptrTail = newNode;
        q->size++;

        return q->size - 1;
    }

    // Queue already has items
    // Find proper location for this item

    int index = 0;
    struct node_t* ptrTraversal = q->ptrHead;

    // OH! I'm not calling the compare function...
    // I keep thinking in C++ terms, argh!! >:|

    while ( ptrTraversal != NULL ) //nodeData >= currentData )
    {
        // Is the item at this index > our new data?
        int pos = q->compareFunction( newNode->data, ptrTraversal->data );

        if ( pos < 0 )
        {
            // New node is "less than" - higher priority. Put ahead.
            break;
        }

        // Otherwise, go to the next thing...

        // Keep traversing while the new node's data
        // is larger than the current data.
        ptrTraversal = ptrTraversal->ptrNext;

        index++;
    }

    if ( ptrTraversal == NULL )
    {
//        DOut( "Put at end of queue" );
        // We've run to the end of the list.
        // New node goes at the end.

        q->ptrTail->ptrNext = newNode;
        newNode->ptrPrev = q->ptrTail;
        q->ptrTail = newNode;

        q->size++;
        return q->size - 1;
    }

    else if ( ptrTraversal == q->ptrHead )
    {
        // Inserting to the beginning of the list

        newNode->ptrNext = q->ptrHead;
        q->ptrHead->ptrPrev = newNode;
        q->ptrHead = newNode;

        q->size++;
        return 0;
    }

    // Otherwise, we're inserting between two nodes.
    // ptrTraversal goes after newNode

//    DOut( "insert node between two nodes" );

    struct node_t* ptrPrev = ptrTraversal->ptrPrev;

    newNode->ptrPrev = ptrPrev;
    newNode->ptrNext = ptrTraversal;

    ptrTraversal->ptrPrev = newNode;
    ptrPrev->ptrNext = newNode;

    q->size++;

    return index;
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
    if ( q->ptrHead == NULL )
    {
        return NULL;
    }

    // Get the data at the front of the queue.
    void* data = q->ptrHead->data;

    return data;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
    return priqueue_remove_at( q, 0 );
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
    // Return null if invalid index or queue is empty.
    if ( index < 0 || index >= q->size  )
    {
        return NULL;
    }

    // Walk to the appropriate node
    struct node_t* ptrTraversal = q->ptrHead;
    for ( int i = 0; i < index; i++ )
    {
        ptrTraversal = ptrTraversal->ptrNext;
    }

    // Return data at this position
    return ptrTraversal->data;
}


/**
  Removes all instances of ptr from the queue.

  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
    int* ptrParam = (int*)ptr;

    if ( ptrParam == NULL || q->size == 0 )
    {
        return 0;
    }

	int* removeIndices = malloc( q->size * sizeof( int ) );
	int removeCount = 0;

    int value = *ptrParam;

    // Find items first
    struct node_t* ptrTraversal = q->ptrHead;
    int counter = 0;
    while ( ptrTraversal != NULL )
    {
        // TODO: I don't think these should be cast,
        // but not sure how to compare two void ptrs
        // values otherwise.
        int nodeValue = *(int*)(ptrTraversal->data);

        if ( nodeValue == value )
        {
            removeIndices[removeCount] = counter;
            removeCount++;
        }

        ptrTraversal = ptrTraversal->ptrNext;
        counter++;
    }

    // Remove items from list
    for ( int i = removeCount-1; i >= 0; i-- )
    {
        priqueue_remove_at( q, removeIndices[i] );
    }

    free( removeIndices );
	return removeCount;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
    // Invalid index
    if ( index < 0 || index >= q->size )
    {
        return NULL;
    }

    // Traverse to that item
    struct node_t* ptrTraversal = q->ptrHead;
    for ( int i = 0; i < index; i++ )
    {
        ptrTraversal = ptrTraversal->ptrNext;
    }

    // Back up the data
    void* data = ptrTraversal->data;
    struct node_t* deleteNode = ptrTraversal;

    if ( ptrTraversal == q->ptrHead && ptrTraversal == q->ptrTail )
    {
        // one item in list
        q->ptrHead = NULL;
        q->ptrTail = NULL;
    }
    else if ( ptrTraversal == q->ptrHead )
    {
        // first item
        q->ptrHead = q->ptrHead->ptrNext;
        q->ptrHead->ptrPrev = NULL;
    }
    else if ( ptrTraversal == q->ptrTail )
    {
        // last item
        q->ptrTail = q->ptrTail->ptrPrev;
        q->ptrTail->ptrNext = NULL;
    }
    else
    {
        // skip over this node
        ptrTraversal->ptrPrev->ptrNext = ptrTraversal->ptrNext;
        ptrTraversal->ptrNext->ptrPrev = ptrTraversal->ptrPrev;
    }

    free( deleteNode );
    q->size--;

	return data;
}


/**
  Returns the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
    return q->size;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
    while ( q->size > 0 )
    {
        // remove head
        priqueue_poll( q );
    }
}

void priqueue_display( priqueue_t *q )
{
    for ( int i = 0; i < q->size; i++ )
    {
        job_t* itemPtr = ((job_t*)priqueue_at( q, i ));

        if ( itemPtr != NULL )
        {
            printf( "Queue item %d  \t", i );
            printf( "\t Job ID:   %d ", itemPtr->job_id );
            printf( "\t Running on core:  %d ", itemPtr->core );
            printf( "\t Arrival time:     %d ", itemPtr->arrival_time );
            printf( "\t Remaining time:   %d ", itemPtr->remaining_time );
            printf( "\t Wait time:   %d ", itemPtr->wait_time );
            printf( "\t Processing time:  %d ", itemPtr->processing_time );
            printf( "\t Turnaround time:  %d ", itemPtr->turnaround_time );
            printf( "\t Total time:   %d \n", itemPtr->total_time );
        }
    }
    printf( "\n" );
}

