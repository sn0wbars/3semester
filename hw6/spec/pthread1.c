#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mylib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <bufrw.h>
#include "matrix.h"

const char fpath[] = "data.txt";

#define DUMP 0

const int defaultN = 3;

int main(int argc, char* argv[])
{
	int y = (argc <= 1) ? defaultN : atoi(argv[1]); // argc ==2
	if (y == 0) 
		printf("number of rows can't be equal 0\n");
	int x = (argc >= 3) ? atoi(argv[2]) : y;
	if (x == 0)
		printf("number of columns can't be equal 0\n");

	struct matrix* matrix1 = matrix_ctor(y,x,NULL);
	if (matrix1 == NULL)
		PERRET1(matrix_ctor);
	struct matrix* matrix2 = matrix_ctor(x,y,NULL); 
	if (matrix1 == NULL)
		PERRET1(matrix_ctor);

	if (matrix_rand(matrix1, 1000) < 0) 
		PERRET1(matrix_rand);

	if (matrix_rand(matrix2, 1000) < 0) 
		PERRET1(matrix_rand);

	if (DUMP)
	{
		if (matrix_printf(matrix1) < 0) 
			PERRET1(matrix_printf);
		if (matrix_printf(matrix2) < 0)
			PERRET1(matrix_printf);	
	}

	int fd = open(fpath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1) 
		PERRET1(open);

	if ((matrix_fprint(fd, matrix1)) < 0) //todo: перегрузить bufwrite
		PERRET1(matrix_fprint);	
	if (matrix_fprint(fd, matrix2) < 0)
		PERRET1(matrix_fprint);	

	if (close(fd) < 0)
		PERRET1(close);

	if (matrix_dctor(matrix1) < 0) 
		PERRET1(matrix_dctor);
	matrix1 = NULL;

	if (matrix_dctor(matrix2) < 0)
		PERRET1(matrix_dctor);
	matrix2 = NULL;

	return 0;
}
