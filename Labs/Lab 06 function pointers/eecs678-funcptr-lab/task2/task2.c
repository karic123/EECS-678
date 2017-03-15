#include <stdio.h>
#include <stdlib.h>

typedef int (*operation_ptr)(int, int);

int add (int a, int b);
int subtract (int a, int b);
int multiply (int a, int b);
int divide (int a, int b);

void GetAandB( int* a, int* b );
int GetOperation();

int main (void)
{
	int a, b;
	int operation;
	operation_ptr operations[] = { add, subtract, multiply, divide };

	GetAandB( &a, &b );
	operation = GetOperation();

	int result = operations[ operation ]( a, b );

	printf( "x = %d \n", result );

	return 0;
}

void GetAandB( int* a, int* b )
{
	printf( "Operand 'a' : " );
	scanf( "%d", a );

	printf( "Operand 'b' : " );
	scanf( "%d", b );
}

int GetOperation()
{
	int operation;

	printf( "Specify the operation to perform (0 : add | 1 : subtract | 2 : multiply | 3 : divide): ");
	scanf( "%d", &operation );

	return operation;
}

int add (int a, int b)
{
	printf ("Adding 'a' and 'b'\n");
	return a + b;
}

int subtract (int a, int b)
{
	printf ("Subtracting 'a' and 'b'\n");
	return a - b;
}

int multiply (int a, int b)
{
	printf ("Multiplying 'a' and 'b'\n");
	return a * b;
}

int divide (int a, int b)
{
	printf ("Dividing 'a' and 'b'\n");
	return a / b;
}
