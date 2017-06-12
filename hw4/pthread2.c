#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <mylib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "nameslib.h"
#include <bufrw.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include "matrix.h"

#define DUMP 0

int defaultM = 1;
char fresult[] = "/home/sn0wbars/Documents/Proga/hw4/result.txt";

int main(int argc, char* argv[])
{
	int m = (argc == 1) ? defaultM : atoi(argv[1]);
	if (m == 0) 
	{
		printf("\'m\' can't be equal 0\n");
		return 1;
	}

	int fd1 = open(fpath, O_RDONLY);
	if (fd1 == -1)
		PERRET1(open);

	char* data = NULL;
	
	int num = bufread(fd1, &data);
	if (num == -1)
		PERRET1(bufread);

	if (close(fd1) < 0)
		PERRET1(close);

	int* data1 = (int*)data;
	int* data2 = (int*)(data + num / 2);

	struct matrix matrix1 = {*data1, *(data1 + 1), data1 + 2};
	struct matrix matrix2 = {*data2, *(data2 + 1), data2 + 2};

	clock_t time1 = time(NULL);
	if (time1 == -1)
		PERRET1(time);

	struct matrix* result = matrix_lelmul(&matrix1, &matrix2, m);
	if (result == NULL)
		PERRET1(matrix_lelmul);

	time1 = time(NULL) - time1;
	printf("Matrixes multiplied %ld seconds\n", time1);

	int fd2 = open(fresult, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd2 == -1) 
		PERRET1(open);

	if (DUMP)
	{
		if (matrix_printf(&matrix1) < 0) 
			PERRET1(matrix_printf);
		if (matrix_printf(&matrix2) < 0)
			PERRET1(matrix_printf);	
	}

	dup2(fd2, 1);
	matrix_printf(result);

	free(data);
	if (matrix_dctor(result) < 0) 
		PERRET1(matrix_dctor);
	result = NULL;

	return 0;
}	
