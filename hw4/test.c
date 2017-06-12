#include <stdio.h>

int foo(int n, int matrix[][n])
{
	int i;
	for (i = 0; i < n; ++i)
		printf("%d ", matrix[0][i]);
	for (i = 0; i < n; ++i)
		printf("%d ", matrix[1][i]);
	return 0;
}

int main()
{
	int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
	foo(5, a);
	return 0;
}