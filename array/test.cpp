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
	printf("%d\n", OrdArray_ctor(&a, 5, *testIntComparator));

	// int a = 123234234;
	printf("%d\n", OrdArray_add(&a, 13));
	printf("%d\n", OrdArray_add(&a, 5));
	printf("%d\n", OrdArray_add(&a, 6));
	printf("%d\n", OrdArray_add(&a, 3));
	printf("%d\n", OrdArray_remove(&a, 5));

	printf("%d\n", OrdArray_add(&a, 3));


	OrdArray_print_int(&a);

	OrdArray_dump(&a);

	// 	printf("Error");
	// //printf("%d\n%d\n%d\n%d", oArr.size, oArr.nsize, oArr.num, oArr.compar);

	// printf("%d\n", *(int*)oArr.array);

	// printf("%d\n", *(int*)OrdArray_index(&oArr, 0));

	//printf("%d\n", d);
	return 0;
}