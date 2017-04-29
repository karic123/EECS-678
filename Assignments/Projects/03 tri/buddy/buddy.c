/**
 * Buddy Allocator
 *
 * For the list library usage, see http://www.mcs.anl.gov/~kazutomo/list/
 */

/**************************************************************************
 * Conditional Compilation Options
 **************************************************************************/
#define USE_DEBUG 1

/**************************************************************************
 * Included Files
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "buddy.h"
#include "list.h"

/**************************************************************************
 * Public Definitions
 **************************************************************************/
#define MIN_ORDER 12
#define MAX_ORDER 20

#define PAGE_SIZE (1<<MIN_ORDER)
/* page index to address */
#define PAGE_TO_ADDR(page_idx) (void *)((page_idx*PAGE_SIZE) + g_memory)

/* address to page index */
#define ADDR_TO_PAGE(addr) ((unsigned long)((void *)addr - (void *)g_memory) / PAGE_SIZE)

/* find buddy address */
#define BUDDY_ADDR(addr, o) (void *)((((unsigned long)addr - (unsigned long)g_memory) ^ (1<<o)) \
									 + (unsigned long)g_memory)

#if USE_DEBUG == 1
#  define PDEBUG(fmt, ...) \
	fprintf(stderr, "%s(), %s:%d: " fmt,			\
		__func__, __FILE__, __LINE__, ##__VA_ARGS__)
#  define IFDEBUG(x) x
#else
#  define PDEBUG(fmt, ...)
#  define IFDEBUG(x)
#endif

/**************************************************************************
 * Rachel's Debug Stuff
 **************************************************************************/
const bool RACH_DEBUG = false;

void DOut( char* functionName, char* message )
{
    if ( !RACH_DEBUG ) { return; }
    printf( "** DEBUG [%s]:\t%s \n", functionName, message );
}

void DOutLbl( char* functionName, char* message, int value )
{
    if ( !RACH_DEBUG ) { return; }
    printf( "** DEBUG [%s]:\t%s \t = %d \n", functionName, message, value );
}

void DOutFunc( char* functionName, bool functionBegin )
{
    if ( !RACH_DEBUG ) { return; }
    if ( functionBegin ) {
        printf( "\n** DEBUG [%s]: FUNCTION BEGIN \n", functionName ); }
    else {
        printf( "\n** DEBUG [%s]: FUNCTION END \n\n", functionName ); }
}

/**************************************************************************
 * Public Types
 **************************************************************************/


/**************************************************************************
 * Global Variables
 **************************************************************************/
/* free lists*/
/*
    This is a list of lists...
    each element of this list is a list of pages for that order (I think).

    So if we're at a point where one block is the entire space: 2^20 B,
    then I assume the first element of this list will point to g_pages[0],
    and the rest will be NULL since we only have one block?

    Minimum order is 12, so at order 12, the page blocks will be 2^12, or
    4096 B each. The max size is 1,048,576 B, so we will have 1048576/4096
    blocks, or 256 total blocks...
*/

struct list_head free_area[ MAX_ORDER+1 ];

/* memory area */
/* This is the "actual memory" that the pages are pointing to. (I think) */
char g_memory[1<<MAX_ORDER]; // THIS IS BASICALLY 2^MAX_ORDER.

/* page structures */
/*
    Total pages is 256, and each page chunk is PAGE_SIZE (4096, or 4K);
    Minimum order is 2^12, or 4096. Max order is 2^20, or 1048576.
*/
 page_t g_pages[(1<<MAX_ORDER)/PAGE_SIZE];

/**************************************************************************
 * Public Function Prototypes
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

/**
 * Initialize the buddy system
 *
 * FIRST we will need to allocate memory for the default list,
 * which will start off as the max order list... the level with
 * only one block, taking up all the memory.
 * Later on, this will be split up, as-needed.
 * pages will point to locations in memory (g_memory)...
 */
const int N_PAGES = (1<<MAX_ORDER) / PAGE_SIZE;

void buddy_init()
{
    DOutFunc( "buddy_init", true );

	int i;
	// Left shifting is multiplying by 2^K
	int n_pages = (1<<MAX_ORDER) / PAGE_SIZE;

    // Initialize the g_pages list
	DOut( "buddy_init", "Initialize g_pages..." );
	for (i = 0; i < n_pages; i++) {
        g_pages[i].ptr_memory = PAGE_TO_ADDR( i );
	}

	/* initialize freelist */
	DOut( "buddy_init", "Initialize free_area list..." );
	for (i = MIN_ORDER; i <= MAX_ORDER; i++) {
		INIT_LIST_HEAD(&free_area[i]);
	}

	/* add the entire memory as a freeblock */
	// This is adding g_pages[0].list to the free_area[MAX_ORDER] list.
	DOut( "buddy_init", "Add entire memory add free block..." );
	list_add( &g_pages[0].list, &free_area[MAX_ORDER] );

	DOutLbl( "buddy_init", "n_pages", n_pages );
	DOutLbl( "buddy_init", "MAX_ORDER", MAX_ORDER );
	DOutLbl( "buddy_init", "PAGE_SIZE", PAGE_SIZE );
}

/**
 * Allocate a memory block.
 *
 * On a memory request, the allocator returns the head of a free-list of the
 * matching size (i.e., smallest block that satisfies the request). If the
 * free-list of the matching block size is empty, then a larger block size will
 * be selected. The selected (large) block is then splitted into two smaller
 * blocks. Among the two blocks, left block will be used for allocation or be
 * further splitted while the right block will be added to the appropriate
 * free-list.
 *
 * @param size size in bytes
 * @return memory block address
 */
void * buddy_alloc(int size)
{
    /*
	struct list_head free_area[MAX_ORDER+1];
    char g_memory[1<<MAX_ORDER];
    page_t g_pages[(1<<MAX_ORDER)/PAGE_SIZE];
	*/

    DOutFunc( "buddy_alloc", true );
	DOutLbl( "buddy_alloc", "Allocate size", size );

    //    1. Ascertain the free-block order which can satisfy the requested size.
    //    The block order for size x is ceil ( log2 (x))
    //    Example: 60k -> block-order = ceil ( log2 (60k)) = ceil ( log2 (k x 2^5 x 2^10)) = order-16

	int requiredOrder = ceil( log2( size ) );
	DOutLbl( "buddy_alloc", "requiredOrder", requiredOrder );

	if ( requiredOrder > MAX_ORDER )
	{
        // Not enough space
        DOut( "buddy_alloc", "Not enough space" );
        return NULL;
	}

    //	2. Iterate over the free-lists; starting from the order calculated
    //	in the above step. If the free-list at the required order is not-empty,
    //	just remove the first page from that list and return it to caller to
    //	satisfy the request
	int order = -1;
	for ( order = requiredOrder; order <= MAX_ORDER; order++ )
	{
        //    3. If the free-list at the required order is empty,
        //    find the first non-empty free-list with order > required-order.
        //    Lets say that such a list exists at order-k
        if ( list_empty( &free_area[ order ] ) )
        {
            // I guess we can't use it like this?
        }

        else
        {
            // Get first page
            page_t* firstPage = list_entry( free_area[ order ].next, page_t, list );
            firstPage->order = requiredOrder;

            // Remove it from the list
            //	4. Remove a page from the order-k list and repeatedly break the page
            //	and populate the respective free-lists until the page of required-order
            //	is obtained. Return that page to caller (It would be good to encase
            //	this functionality in a separate function e.g. split)
            list_del( &(firstPage->list) );
            breakdown( firstPage, order, requiredOrder );

            return ((void*) firstPage->ptr_memory );
        }

	}


    //    5. If a non-empty free-list is not found, this is an error
	return NULL;
}

/**
 * Free an allocated memory block.
 *
 * Whenever a block is freed, the allocator checks its buddy. If the buddy is
 * free as well, then the two buddies are combined to form a bigger block. This
 * process continues until one of the buddies is not free.
 *
 * @param addr memory block address to be freed
 */

void buddy_free(void * addr)
{
    DOutFunc( "buddy_free", true );

    //    1. Calculate the address of the buddy
    page_t* deleteMePage = &g_pages[ ADDR_TO_PAGE( addr ) ];
    int order = deleteMePage->order;

    if( order < MIN_ORDER )
    {
        printf("Error-Page order less than specified MIN_ORDER %d", order);
        return;
    }
    else if( order > MAX_ORDER )
    {
        printf("Error-Page order greater than specified MAX_ORDER %d", order);
        return;
    }

    //    2. If the buddy is free, merge the two blocks i.e. remove the buddy
    //    from its free-list, update the order of the page-at-hand and add
    //    the page to the relevant free-list

    //    3. Do step-2 repeatedly until no merging is possible
    //        a. The buddy is not free
    //        b. The max order is reached
    for ( order = deleteMePage->order; order < MAX_ORDER; order++ ) // Goes until scenario 3b
    {
        DOutLbl( "buddy_free", "Current order:", order );
        page_t* buddyPage = &g_pages[ ADDR_TO_PAGE( BUDDY_ADDR( deleteMePage->ptr_memory, order ) ) ];

        // Is the buddy free?
        bool buddyIsFree = false;
        struct list_head* it; // iterator

        // #define list_for_each(pos, head) ...
        //        for (pos = (head)->next; pos != (head); ...
        //                pos = pos->next)
        // Ugh I hate fucking macro "magic code".
        list_for_each( it, &free_area[ order ] )
        {
            // Can we find our buddy?
            if ( list_entry( it, page_t, list ) == buddyPage )
            {
                buddyIsFree = true;
            }
        }

        if ( !buddyIsFree )
        {
            // Scenario 3a
            // Nothing more to do, can't merge our blocks.
            break;
        }

        // remove the buddy from its free-list
        list_del( &( buddyPage->list ) );

        if ( buddyPage < deleteMePage )
        {
            deleteMePage = buddyPage;
        }
    }

    // update the order of the page-at-hand
    deleteMePage->order = order;

    // add the page to the relevant free-list
    list_add( &( deleteMePage->list ), &free_area[ order ] );
}

/**
 * Print the buddy system status---order oriented
 *
 * print free pages in each order.
 */
void buddy_dump()
{
    DOutFunc( "buddy_dump", true );

	int o;
	for (o = MIN_ORDER; o <= MAX_ORDER; o++) {
		struct list_head *pos;
		int cnt = 0;
		list_for_each(pos, &free_area[o]) {
			cnt++;
		}
		printf("%d:%dK ", cnt, (1<<o)/1024);
	}
	printf("\n");
}


void breakdown( page_t* page, int order, int requiredOrder )
{
    DOutFunc( "breakdown", true );

    if ( order == requiredOrder ) { return; /* Done with splittin' */ }
    int adjustedOrder = order - 1;
	DOutLbl( "breakdown", "Current adjustedOrder:", adjustedOrder );

    // Split in half recursively
    page_t* secondHalf = &( g_pages[
        ADDR_TO_PAGE(
        BUDDY_ADDR( page->ptr_memory, adjustedOrder ) ) ] );
    secondHalf->order = adjustedOrder;

    list_add( &( secondHalf->list ), &free_area[ adjustedOrder ] );

    // Do it again!
    breakdown( page, adjustedOrder, requiredOrder );
}
