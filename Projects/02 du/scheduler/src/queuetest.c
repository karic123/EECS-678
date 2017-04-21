/** @file queuetest.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libpriqueue/libpriqueue.h"

const bool RACH_TEST = true;

int TestInit();
int TestPushBack();
int TestOffer();
int TestPeek();
int TestPoll();
int TestAt();
int TestRemoveAt();
int TestRemove();
int TestSize();
int TestDestroy();

void DTestHeader( char* test )
{
    if ( RACH_TEST )
    {
    }
}

void DTestFail( char* test, char* message )
{
    if ( RACH_TEST )
    {
        printf( "\n  TEST FAILED [%s]: %s \n\n", test, message );
    }
}

void DTestFailEx( char* test, char* message, int expected, int actual )
{
    if ( RACH_TEST )
    {
        printf( "\n  TEST FAILED [%s]: %s, \n\t Expected: %d, Actual: %d \n", test, message, expected, actual );
    }
}

void DTestPass( char* test )
{
    if ( RACH_TEST )
    {
    }
}

void DTestResult( char* test, int score, int total )
{
    if ( RACH_TEST )
    {
        if ( score == total )
        {
            printf( "o  " );
        }
        else
        {
            printf( "x  " );
        }
        printf( "SCORE: %d / %d, \t TEST: %s \n", score, total, test );
    }
}

int compare1(const void * a, const void * b)
{
    int value1 = *(int*)a;
    int value2 = *(int*)b;
    int result = value1 - value2;

	return result;
}

int compare2(const void * a, const void * b)
{
    int value1 = *(int*)a;
    int value2 = *(int*)b;
    int result = value2 - value1;

	return result;
}


int main()
{
    srand( time( NULL ) );

    int score = 0;
    int total = 0;

    score += TestInit();        total++;
    score += TestPeek();        total++;
    score += TestPoll();        total++;
    score += TestAt();          total++;
    score += TestRemove();      total++;
    score += TestRemoveAt();    total++;
    score += TestSize();        total++;
    score += TestDestroy();     total++;
    score += TestPushBack();    total++;
    score += TestOffer();       total++;

    DTestResult( "ALL TESTS", score, total );

	return 0;
}


/*
  Initializes the priqueue_t data structure.
    Prerequisite:
        void   priqueue_init
        void   priqueue_destroy

    Init: Set head and tail to NULL
*/
int TestInit()
{
    DTestHeader( "TestInit() \n" );
    int passed = 0;
    int totalTests = 0;

	priqueue_t q;

	priqueue_init(&q, compare1);

    totalTests++;
	if ( q.ptrHead == NULL )
	{
        passed++;
	}
	else
	{
        DTestFail( "TestInit()", "ptrHead was not NULL." );
	}

    totalTests++;
	if ( q.ptrTail == NULL )
	{
        passed++;
	}
	else
	{
        DTestFail( "TestInit()", "ptrTail was not NULL." );
	}

    totalTests++;
    int size = priqueue_size( &q );
    int expectedSize = 0;
	if ( size == expectedSize )
	{
        passed++;
	}
	else
	{
        DTestFailEx( "TestInit()",
            "Size for new queue was not correct.", expectedSize, size );
	}

	priqueue_destroy(&q);

    // Results
    DTestResult( "TestInit()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestInit()" ); }
	return ( passed == totalTests );
}

/*
    Prerequisites:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_push_back
        void * priqueue_at

        *((int *)priqueue_at(&q2, i))
*/
int TestPushBack()
{
    DTestHeader( "TestPushBack() \n" );
    int passed = 0;
    int totalTests = 0;

    // Test 1: Push several items to the queue
	priqueue_t q;
	priqueue_init(&q, compare1);

	int insertValues[] = { 2, 4, 6, 8 };

	for ( int i = 0; i < 4; i++ )
	{
        priqueue_push_back( &q, &insertValues[i] );

        // Check to make sure the value is correct
        int* itemPtr = ((int*)priqueue_at( &q, i ));
        totalTests++;
        if ( *itemPtr != insertValues[i] )
        {
            DTestFailEx( "TestPushBack()", "A: Incorrect value of newly inserted item",
                insertValues[i], *itemPtr );
        }
        else
        {
            passed++;
        }
	}

	priqueue_destroy(&q);


    // Test results
    DTestResult( "TestPushBack()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestPushBack()" ); }
	return ( passed == totalTests );
}

/*
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure

  @param ptr a pointer to the data to be inserted into the priority queue

  @return The zero-based index where ptr is stored in the priority queue,
    where 0 indicates that ptr was stored at the front of the priority queue.

    Prerequisites:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_offer
        void * priqueue_at

        *((int *)priqueue_at(&q2, i))
*/
int TestOffer()
{
    DTestHeader( "TestOffer() \n" );
    int passed = 0;
    int totalTests = 0;
	priqueue_t q;
    int values[] = { 3, 4, 5, 6, 7 };
    int returnValue;
    int head;
    int tail;
    int valueAt, valueAtA, valueAtB, valueAtC;

    // Test 1: Add one item to queue and validate position
	priqueue_init( &q, compare1 );

	returnValue = priqueue_offer( &q, &values[0] );

	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAt = *((int *)priqueue_at( &q, 0 ));

    if ( q.size != 1 )
    {
        DTestFailEx( "TestOffer() Test1", "Incorrect size after insert", 1, q.size );
    }
	else if ( returnValue != 0 )
	{
        DTestFailEx( "TestOffer() Test1", "Incorrect index returned by offer", 0, returnValue );
	}
	else if ( head != values[0] )
	{
        DTestFailEx( "TestOffer() Test1", "Incorrect head value", values[0], head );
	}
	else if ( tail != values[0] )
	{
        DTestFailEx( "TestOffer() Test1", "Incorrect tail value", values[0], tail );
	}
	else if ( valueAt != values[0] )
	{
        DTestFailEx( "TestOffer() Test1", "Incorrect value at position 0", values[0], valueAt );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);

	// Test 2: Insert to head (asc)
	priqueue_init( &q, compare1 );

	// Insert first item
	returnValue = priqueue_offer( &q, &values[2] );

	// Insert item with higher priority (lower number) and verify
	// Since this item is higher priority, it should be inserted to position 0.
	returnValue = priqueue_offer( &q, &values[1] );

	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAtA = *((int *)priqueue_at( &q, 0 ));
	valueAtB = *((int *)priqueue_at( &q, 1 ));

    if ( q.size != 2 )
    {
        DTestFailEx( "TestOffer() Test2", "Incorrect size after insert", 2, q.size );
    }
	else if ( returnValue != 0 )
	{
        DTestFailEx( "TestOffer() Test2", "Incorrect index returned by offer", 0, returnValue );
	}
	else if ( head != values[1] )
	{
        DTestFailEx( "TestOffer() Test2", "Incorrect head value", values[1], head );
	}
	else if ( tail != values[2] )
	{
        DTestFailEx( "TestOffer() Test2", "Incorrect tail value", values[2], tail );
	}
	else if ( valueAtA != values[1] )
	{
        DTestFailEx( "TestOffer() Test2", "Incorrect value at position 0", values[1], valueAtA );
	}
	else if ( valueAtB != values[2] )
	{
        DTestFailEx( "TestOffer() Test2", "Incorrect value at position 1", values[2], valueAtB );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);

	// Test 3: Insert to tail (asc)
	priqueue_init( &q, compare1 );

	// Insert first item
	returnValue = priqueue_offer( &q, &values[2] );

	// Insert item with lower priority (higher number) and verify
	// Since this item is higher priority, it should be inserted to position 1.
	returnValue = priqueue_offer( &q, &values[3] );

	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAtA = *((int *)priqueue_at( &q, 0 ));
	valueAtB = *((int *)priqueue_at( &q, 1 ));

    if ( q.size != 2 )
    {
        DTestFailEx( "TestOffer() Test3", "Incorrect size after insert", 2, q.size );
    }
	else if ( returnValue != 1 )
	{
        DTestFailEx( "TestOffer() Test3", "Incorrect index returned by offer", 1, returnValue );
	}
	else if ( head != values[2] )
	{
        DTestFailEx( "TestOffer() Test3", "Incorrect head value", values[2], head );
	}
	else if ( tail != values[3] )
	{
        DTestFailEx( "TestOffer() Test3", "Incorrect tail value", values[3], tail );
	}
	else if ( valueAtA != values[2] )
	{
        DTestFailEx( "TestOffer() Test3", "Incorrect value at position 0", values[2], valueAtA );
	}
	else if ( valueAtB != values[3] )
	{
        DTestFailEx( "TestOffer() Test3", "Incorrect value at position 1", values[3], valueAtB );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);


	// Test 4: Insert between two items (asc)
	priqueue_init( &q, compare1 );

	// Insert first item
	returnValue = priqueue_offer( &q, &values[1] ); // 0
	returnValue = priqueue_offer( &q, &values[3] ); // 1

	// Insert item with lower priority (higher number) and verify
	// Since this item is higher priority, it should be inserted to position 1.
	returnValue = priqueue_offer( &q, &values[2] );

	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAtA = *((int *)priqueue_at( &q, 0 )); // values[1]
	valueAtB = *((int *)priqueue_at( &q, 1 )); // values[2]
	valueAtC = *((int *)priqueue_at( &q, 2 )); // values[3]

    if ( q.size != 3 )
    {
        DTestFailEx( "TestOffer() Test4", "Incorrect size after insert", 3, q.size );
    }
	else if ( returnValue != 1 )
	{
        DTestFailEx( "TestOffer() Test4", "Incorrect index returned by offer", 1, returnValue );
	}
	else if ( head != values[1] )
	{
        DTestFailEx( "TestOffer() Test4", "Incorrect head value", values[1], head );
	}
	else if ( tail != values[3] )
	{
        DTestFailEx( "TestOffer() Test4", "Incorrect tail value", values[3], tail );
	}
	else if ( valueAtA != values[1] )
	{
        DTestFailEx( "TestOffer() Test4", "Incorrect value at position 0", values[1], valueAtA );
	}
	else if ( valueAtB != values[2] )
	{
        DTestFailEx( "TestOffer() Test4", "Incorrect value at position 1", values[2], valueAtB );
	}
	else if ( valueAtC != values[3] )
	{
        DTestFailEx( "TestOffer() Test4", "Incorrect value at position 1", values[3], valueAtC );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);


	// Do the same tests in reverse!


	// Test 2b: Insert to head (desc)
	priqueue_init( &q, compare2 );

	// Insert first item
	returnValue = priqueue_offer( &q, &values[2] ); // insert at 0

	// Value 2 is larger than Value 1. With compare2,
	// in this case, value 2 will go first.
	returnValue = priqueue_offer( &q, &values[1] ); // insert at 1


	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAtA = *((int *)priqueue_at( &q, 0 ));
	valueAtB = *((int *)priqueue_at( &q, 1 ));

    if ( q.size != 2 )
    {
        DTestFailEx( "TestOffer() Test2B", "Incorrect size after insert", 2, q.size );
    }
	else if ( returnValue != 1 )
	{
        DTestFailEx( "TestOffer() Test2B", "Incorrect index returned by offer", 1, returnValue );
	}
	else if ( head != values[2] )
	{
        DTestFailEx( "TestOffer() Test2B", "Incorrect head value", values[2], head );
	}
	else if ( tail != values[1] )
	{
        DTestFailEx( "TestOffer() Test2B", "Incorrect tail value", values[1], tail );
	}
	else if ( valueAtA != values[2] )
	{
        DTestFailEx( "TestOffer() Test2B", "Incorrect value at position 0", values[2], valueAtA );
	}
	else if ( valueAtB != values[1] )
	{
        DTestFailEx( "TestOffer() Test2B", "Incorrect value at position 1", values[1], valueAtB );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);




	// Test 3b: Insert to tail (desc)
	priqueue_init( &q, compare2 );

	// Insert first item
	returnValue = priqueue_offer( &q, &values[2] ); // Insert to 0

	returnValue = priqueue_offer( &q, &values[3] ); // Larger value: Go to 0

	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAtA = *((int *)priqueue_at( &q, 0 ));
	valueAtB = *((int *)priqueue_at( &q, 1 ));

    if ( q.size != 2 )
    {
        DTestFailEx( "TestOffer() Test3B", "Incorrect size after insert", 2, q.size );
    }
	else if ( returnValue != 0 )
	{
        DTestFailEx( "TestOffer() Test3B", "Incorrect index returned by offer", 0, returnValue );
	}
	else if ( head != values[3] )
	{
        DTestFailEx( "TestOffer() Test3B", "Incorrect head value", values[3], head );
	}
	else if ( tail != values[2] )
	{
        DTestFailEx( "TestOffer() Test3B", "Incorrect tail value", values[2], tail );
	}
	else if ( valueAtA != values[3] )
	{
        DTestFailEx( "TestOffer() Test3B", "Incorrect value at position 0", values[3], valueAtA );
	}
	else if ( valueAtB != values[2] )
	{
        DTestFailEx( "TestOffer() Test3B", "Incorrect value at position 1", values[2], valueAtB );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);


	// Test 4b: Insert between two items (desc)
	priqueue_init( &q, compare2 );

	// Insert first item
	returnValue = priqueue_offer( &q, &values[1] );
	returnValue = priqueue_offer( &q, &values[3] );
	returnValue = priqueue_offer( &q, &values[2] );

	// head should be largest value, values[3]
	// tail should be smallest value, values[1].

	// Check
	totalTests++;
	head = *((int *)q.ptrHead->data);
	tail = *((int *)q.ptrTail->data);
	valueAtA = *((int *)priqueue_at( &q, 0 )); // values[3]
	valueAtB = *((int *)priqueue_at( &q, 1 )); // values[2]
	valueAtC = *((int *)priqueue_at( &q, 2 )); // values[1]

    if ( q.size != 3 )
    {
        DTestFailEx( "TestOffer() Test4B", "Incorrect size after insert", 3, q.size );
    }
	else if ( returnValue != 1 )
	{
        DTestFailEx( "TestOffer() Test4B", "Incorrect index returned by offer", 1, returnValue );
	}
	else if ( head != values[3] )
	{
        DTestFailEx( "TestOffer() Test4B", "Incorrect head value", values[3], head );
	}
	else if ( tail != values[1] )
	{
        DTestFailEx( "TestOffer() Test4B", "Incorrect tail value", values[1], tail );
	}
	else if ( valueAtA != values[3] )
	{
        DTestFailEx( "TestOffer() Test4B", "Incorrect value at position 0", values[3], valueAtA );
	}
	else if ( valueAtB != values[2] )
	{
        DTestFailEx( "TestOffer() Test4B", "Incorrect value at position 1", values[2], valueAtB );
	}
	else if ( valueAtC != values[1] )
	{
        DTestFailEx( "TestOffer() Test4B", "Incorrect value at position 1", values[1], valueAtC );
	}
	else
	{
        passed++;
	}

	priqueue_destroy(&q);


    // Test results
    DTestResult( "TestOffer()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestOffer()" ); }
	return ( passed == totalTests );
}

/*
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
    Prerequisites:
        void   priqueue_init
        void   priqueue_destroy
        void * priqueue_peek
        int    priqueue_offer
*/
int TestPeek()
{
    DTestHeader( "TestPeek() \n" );
    int passed = 0;
    int totalTests = 0;


    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);
	int *values = malloc(100 * sizeof(int));
	int i;
	for (i = 0; i < 100; i++)
	{
		values[i] = rand() % 100;
    }

	// Tests
	// Test 1: Empty
	totalTests++;
	int* ptrValue = ((int*)priqueue_peek(&q));
	if ( ptrValue != NULL )
	{
        DTestFail( "TestPeek()", "Incorrect queue head, didn't return NULL ptr for empty queue." );
	}
	else
	{
        passed++;
	}

	// Test 2: One item
	priqueue_push_back( &q, &values[0] );

	totalTests++;
	ptrValue = ((int*)priqueue_peek(&q));
	int expectedValue = values[0];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestPeek()", "Incorrect queue head, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestPeek()", "Incorrect queue head value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

	// Several items
	priqueue_push_back( &q, &values[1] );
	priqueue_push_back( &q, &values[2] );

	totalTests++;
	ptrValue = ((int*)priqueue_peek(&q));
	expectedValue = values[0];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestPeek()", "Incorrect queue head, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestPeek()", "Incorrect queue head value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

    // Cleanup
	priqueue_destroy(&q);
	free(values);

    // Results
    DTestResult( "TestPeek()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestPeek()" ); }
	return ( passed == totalTests );
}

/*
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
    Prerequisites:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_offer
        void * priqueue_poll
        int    priqueue_size
*/
int TestPoll()
{
    DTestHeader( "TestPoll() \n" );
    int passed = 0;
    int totalTests = 0;


    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);
	int *values = malloc(100 * sizeof(int));
	int i;
	for (i = 0; i < 100; i++)
	{
		values[i] = rand() % 100;
    }

	// Tests
	// Test 1: Empty
	totalTests++;
	int* ptrValue = ((int*)priqueue_poll(&q));
	if ( ptrValue != NULL )
	{
        DTestFail( "TestPoll()", "Incorrect queue head, didn't return NULL ptr for empty queue." );
	}
	else
	{
        passed++;
	}

	// Test 2: One item
	priqueue_push_back( &q, &values[0] );

	totalTests++;
	ptrValue = ((int*)priqueue_poll(&q));
	int expectedValue = values[0];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestPoll()", "Incorrect queue head, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestPoll()", "Incorrect queue head value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

	totalTests++;
	int size = priqueue_size( &q );
	expectedValue = 0;
	if ( size != expectedValue )
	{
        DTestFailEx( "TestPoll()", "Incorrect queue length", expectedValue, size );
	}
	else
	{
        passed++;
	}

	// Test 3: A few items
	priqueue_push_back( &q, &values[5] );
	priqueue_push_back( &q, &values[6] );
	priqueue_push_back( &q, &values[7] );

	totalTests++;
	ptrValue = ((int*)priqueue_poll(&q));
	expectedValue = values[5];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestPoll()", "Incorrect queue head, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestPoll()", "Incorrect queue head value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

	totalTests++;
	size = priqueue_size( &q );
	expectedValue = 2;
	if ( size != expectedValue )
	{
        DTestFailEx( "TestPoll()", "Incorrect queue length", expectedValue, size );
	}
	else
	{
        passed++;
	}

    // Cleanup
	priqueue_destroy(&q);
	free(values);

    // Results
    DTestResult( "TestPoll()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestPoll()" ); }
	return ( passed == totalTests );
}

/*
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
    Prerequisites:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_offer
        void * priqueue_at
*/
int TestAt()
{
    DTestHeader( "TestAt() \n" );
    int passed = 0;
    int totalTests = 0;


    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);
	int *values = malloc(100 * sizeof(int));
	int i;
	for (i = 0; i < 100; i++)
		values[i] = rand() % 100;

	// Tests

	// Test 1: Empty
    totalTests++;
	int* ptrValue = ((int*)priqueue_at(&q, 5));
	if ( ptrValue == NULL )
	{
        passed++;
	}
	else
	{
        DTestFail( "TestAt()", "Incorrect queue at position 5, didn't return NULL ptr for empty queue." );
	}


	// q: in order
	priqueue_push_back(&q, &values[10]); // 0
	priqueue_push_back(&q, &values[30]); // 1
	priqueue_push_back(&q, &values[20]); // 2

    // 0
    totalTests++;
	ptrValue = ((int*)priqueue_at(&q, 0));
	int expectedValue = values[10];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestAt()", "A. Incorrect queue value, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestAt()", "A. Incorrect queue value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

    // 1
    totalTests++;
    ptrValue = ((int*)priqueue_at(&q, 1));
    expectedValue = values[30];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestAt()", "B. Incorrect queue value, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestAt()", "B. Incorrect queue value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

    // 2
    totalTests++;
    ptrValue = ((int*)priqueue_at(&q, 2));
    expectedValue = values[20];
	if ( ptrValue == NULL )
	{
        DTestFail( "TestAt()", "C. Incorrect queue value, returned NULL ptr." );
	}
	else if ( *ptrValue != expectedValue )
	{
        DTestFailEx( "TestAt()", "C. Incorrect queue value", expectedValue, *ptrValue );
	}
	else
	{
        passed++;
	}

	// TestOffer is already testing size

    // Cleanup
	priqueue_destroy(&q);
	free(values);

    // Results
    DTestResult( "TestAt()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestAt()" ); }
	return ( passed == totalTests );
}

/*
  Removes all instances of ptr from the queue.
  @return the number of entries removed
    Prerequisite:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_remove
*/
int TestRemove()
{
    DTestHeader( "TestRemove() \n" );
    int passed = 0;
    int totalTests = 0;

    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);
	int *values = malloc(100 * sizeof(int));
	int i;
	int countOfZeroes = 0;
	for (i = 0; i < 100; i++)
	{
		values[i] = i % 10;
		if ( i % 10 == 0 )
		{
            countOfZeroes++;
		}
		priqueue_push_back( &q, &values[i] );
    }

	// Tests

	// Remove something not in the queue
	totalTests++;
	int value = 20;
	int removeCount = priqueue_remove( &q, (void*)(&value) );
	int expectedValue = 0;
	if ( removeCount == expectedValue )
	{
        passed++;
	}
	else
	{
        DTestFailEx( "TestRemove()", "Incorrect remove count for item not in queue", expectedValue, removeCount );
	}

	// Remove some value
	totalTests++;
	value = 0;
	removeCount = priqueue_remove( &q, (void*)(&value) );
	if ( removeCount == countOfZeroes )
	{
        passed++;
	}
	else
	{
        DTestFailEx( "TestRemove()", "Incorrect remove count", countOfZeroes, removeCount );
	}

    // Cleanup
	priqueue_destroy(&q);
	free(values);

    // Results
    DTestResult( "TestRemove()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestRemove()" ); }
	return ( passed == totalTests );
}

/*
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
    Prerequisite:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_offer
        void * priqueue_at
        int    priqueue_size
*/
int TestRemoveAt()
{
    DTestHeader( "TestRemoveAt() \n" );
    int passed = 0;
    int totalTests = 0;

    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);
	int *values = malloc(100 * sizeof(int));
	int i;
	for (i = 0; i < 100; i++)
	{
		values[i] = i;
    }

    for ( int i = 0; i < 7; i++ )
    {
        priqueue_push_back( &q, &values[i] );
    }

	// Remove item at front, back, middle, and invalid index.
    int inputs[] = { 0, 0, 4, 1 };
    int expectedOutputs[] = { values[0], values[1], values[6], values[4] };
    int expectedSizes[] = { 6, 5, 4, 3 };

    // index    0  1  2  3  4  5  6
    //          -------------------
    // Initial  0, 1, 2, 3, 4, 5, 6     Remove 0 (value = 0)
    // i = 0:   1, 2, 3, 4, 5, 6        Remove 0 (value = 1)
    // i = 1:   2, 3, 4, 5, 6           Remove 4 (value = 6)
    // i = 2:   3, 4, 5, 6              Remove 1 (value = 4)

	// Tests
	for ( int i = 0; i < 3; i++ )
	{
        totalTests++;
        int* removedValue = ( (int*)priqueue_remove_at( &q, inputs[i] ) );
        int size = priqueue_size( &q );
        if ( removedValue == NULL )
        {
            DTestFail( "TestRemoveAt()", "Incorrect removed value, returned NULL ptr." );
        }
        else if ( *removedValue != expectedOutputs[i] )
        {
            DTestFailEx( "TestRemoveAt()", "Incorrect removed value",
                expectedOutputs[i], *removedValue );
        }
        else if ( size != expectedSizes[i] )
        {
            DTestFailEx( "TestRemoveAt()", "Incorrect queue size",
                expectedSizes[i], size );
        }
        else
        {
            passed++;
        }
	}

	// Remove item not in queue
    totalTests++;
	int* removedValue = ( (int*)priqueue_remove_at( &q, 200 ) );
	int size = priqueue_size(&q);
	if ( removedValue != NULL )
	{
        DTestFail( "TestRemoveAt()",
            "Incorrect removed value, item not in list should have returned NULL." );
	}
	else if ( size != 4 )
	{
        DTestFailEx( "TestRemoveAt()",
            "Incorrect size after failed remove", 4, size );
	}
	else
	{
        passed++;
	}

    // Cleanup
	priqueue_destroy(&q);
	free(values);

    // Results
    DTestResult( "TestRemoveAt()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestRemoveAt()" ); }
	return ( passed == totalTests );
}

/*
  Returns the number of elements in the queue.
    Prerequisite:
        void   priqueue_init
        void   priqueue_destroy
        int    priqueue_size
        int    priqueue_offer
*/
int TestSize()
{
    DTestHeader( "TestSize() \n" );
    int passed = 0;
    int totalTests = 0;

    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);
	int *values = malloc(100 * sizeof(int));
	int i;
	for (i = 0; i < 100; i++)
		values[i] = rand() % 100;


	// Test empty
	totalTests++;
	int size = priqueue_size( &q );
	if ( size == 0 )
	{
        passed++;
	}
	else
	{
        DTestFailEx( "TestSize()", "Incorrect queue size", 0, size );
	}

	// Test size 1
	totalTests++;
	priqueue_offer(&q, &values[0]);
    size = priqueue_size( &q );
	if ( size == 1 )
	{
        passed++;
	}
	else
	{
        DTestFailEx( "TestSize()", "Incorrect queue size", 1, size );
	}

	// Test size 5
	totalTests++;
	priqueue_offer(&q, &values[0]);
	priqueue_offer(&q, &values[1]);
	priqueue_offer(&q, &values[2]);
	priqueue_offer(&q, &values[3]);
    size = priqueue_size( &q );
	if ( size == 5 )
	{
        passed++;
	}
	else
	{
        DTestFailEx( "TestSize()", "Incorrect queue size", 5, size );
	}

    // Cleanup
	priqueue_destroy(&q);
	free(values);

    // Results
    DTestResult( "TestSize()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestSize()" ); }
	return ( passed == totalTests );
}

/*
  Destroys and frees all the memory associated with q.

  Prerequisite:
    void   priqueue_init
    void   priqueue_destroy
*/
int TestDestroy()
{
    DTestHeader( "TestDestroy() \n" );
    int passed = 0;
    int totalTests = 0;


    // Init
	priqueue_t q;
	priqueue_init(&q, compare1);

    // Cleanup
    totalTests++;
	priqueue_destroy(&q);
	passed++;

    // If it gets to this point, destroy probably worked...
    // (Otherwise, segfault...)

    // Results
    DTestResult( "TestDestroy()", passed, totalTests );
    if ( passed == totalTests ) { DTestPass( "TestDestroy()" ); }
	return ( passed == totalTests );
}

