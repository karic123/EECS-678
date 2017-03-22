#include <stdio.h>
#include <stdlib.h>

typedef int (*function_ptr)(const void*, const void*);

int thing1(const void* a, const void* b)
{
	printf( "Thing 1 \n" );
	int A = (*(int*)a);
	int B = (*(int*)b);
	
	return A > B;
}

int thing2(const void* a, const void* b)
{
	printf( "Thing 2 \n" );
	int A = (*(int*)a);
	int B = (*(int*)b);
	
	return A < B;
}

struct Potato
{
	int number;
	function_ptr * func;
	int (*funcPtr)(const void*, const void*);
};

void InitPotato( struct Potato* ptrPotato, int(*function)(const void*, const void*) )
{
	ptrPotato->funcPtr = function;
}

void RunPotato( struct Potato* ptrPotato )
{
	int val1 = 10;
	int val2 = 20;
	int result = ptrPotato->funcPtr( (void*)&val1, (void*)&val2 );
	printf( "Result: %d \n", result );
}

int main()
{
	struct Potato p;
	InitPotato( &p, thing1 );
	
	struct Potato q;
	InitPotato( &q, thing2 );
	
	printf( "Potato 1 " );
	RunPotato( &p );
	
	printf( "Potato 2 " );
	RunPotato( &q );
	
	return 0;
}
