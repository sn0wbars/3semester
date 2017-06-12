#include "array.h"
#include <stdio.h>
#include <stdlib.h>


int testIntComparator(const void* e1, const void* e2)
{
	return (*(int*)e1-*(int*)e2);
}

int main()
{
	OrdArray a;
	//printf("1212\n");
	printf("%d", OrdArray_ctor(&a, 5, *testIntComparator));

	// int a = 123234234;
	printf("%d", OrdArray_add(&a, 13));
	printf("%d", OrdArray_add(&a, 5));
	printf("%d", OrdArray_add(&a, 6));
	printf("%d", OrdArray_add(&a, 3));
	printf("\n");
	OrdArray_print_int(&a);

	printf("%d", OrdArray_removeElem(&a, 5));
	printf("%d", OrdArray_removeElem(&a, 10));
	printf("%d", OrdArray_removeElem(&a, 13));

	printf("\n");
	OrdArray_print_int(&a);

	printf("%d", OrdArray_add(&a, 3));
	printf("%d", OrdArray_add(&a, 3));
	printf("%d", OrdArray_add(&a, 1));
	printf("%d", OrdArray_add(&a, 3));
	printf("%d", OrdArray_add(&a, 3));

	printf("\n");
	OrdArray_print_int(&a);

	printf("%d", OrdArray_removeIndx(&a,4));
	printf("%d", OrdArray_removeIndx(&a,1));

	printf("\n");
	OrdArray_print_int(&a);

	printf("%d", OrdArray_removeIndx(NULL, 1));
	printf("%d", OrdArray_add(NULL, 3));
	printf("%d", OrdArray_removeElem(NULL, 13));

	printf("\n");
	OrdArray_print_int(&a);

	return 0;
}