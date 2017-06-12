#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "matrix.h"
#include <bufrw.h>
#include <stdbool.h>	

const bool DUMP_MATRIX = 0;

struct mtxdata
{
	struct matrix* mtx1;
	struct matrix* mtx2;
	struct matrix* result;
	int m;
	int work;
};

struct thrMulArg
{
	struct mtxdata* data;
	int n;
};

void* swap(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

int matrix_rand(struct matrix* this, int max)
{
	if (!matrix_check(this)) 
		return -1;

	int seed = time(NULL) + (long int)this;
	if (seed == -1)
		return -2;
	srand(seed);

	int x = this->x;
	int y = this->y;

	if (max == 0)		
		for(int j = 0; j < y; ++j)
			for (int i = 0; i < x; ++i)
				(this->a)[i + j*x] = rand();
	else
		for(int j = 0; j < y; ++j)
			for (int i = 0; i < x; ++i)
				(this->a)[i + j*x] = rand()%max;
	return 0;
}

int matrix_fprint(int fd, struct matrix* this)
{
	if (!matrix_check(this))
		return -1;

	int dimensions[] = {this->x, this->y};

	if (bufwriteI(fd, (char*)dimensions, (sizeof dimensions)) < 0)
		return -2;	

	if (bufwriteI(fd, (char*)this->a, this->x * this->y * sizeof(*this->a)) < 0) //todo: перегрузить bufwrite
		return -2;
	
	return 0;
}

int matrix_printf(struct matrix* this) // pointer
{
	if (!matrix_check(this))
		return -1;

	int x = this->x;
	int y = this->y;

	printf("%d %d\n", y, x);
	for(int j = 0; j < y; ++j)
	{
		for (int i = 0; i < 13*x; ++i)
			putchar('=');
		printf("\n");
		for (int i = 0; i < x; ++i)
			printf("|%11d| ", this->a[i + j*x]);
		printf("\n");
	}
	for (int i = 0; i < 13*x; ++i)
		putchar('=');
	printf("\n\n");
	return 0;
}	

struct matrix* matrix_ctor(int y, int x, int* a)
{
	int n = x*y;
	struct matrix* this = (struct matrix*)malloc(sizeof (struct matrix));
	if (this)
		if (this->a = (int*)calloc(n, (int)sizeof(*a))); 
		{
			this->x = x;
			this->y = y;
			if (a)
				for(int i = 0; i < n; ++i)
					this->a[i] = a[i];
			return this;
		}
	return NULL;
}

struct matrix* matrix_copy_ctor(struct matrix* this)
{
	if (!matrix_check(this))
		return NULL;

	struct matrix* copy = matrix_ctor(this->y, this->x, this->a);
	return copy;
}

int matrix_dctor(struct matrix* this)
{
	if (!matrix_check(this))
		return -1;

	free (this->a);
	this->a = NULL;
	this->x = -1;
	this->y = -1;
	free (this);
	return 0;
}

int matrix_check(struct matrix* this)
{
	if (this == NULL) 
	{
		errno = EFAULT;
		return 0;
	}

	else if (this->a == NULL || this->x < 0 || this->y < 0) 
	{
		errno = EINVAL;
		return 0;
	}

	else return 1;
}

int matrix_transpose(struct matrix* this)
{
	if (!matrix_check(this))
		return -1;

	struct matrix* copy = matrix_copy_ctor(this);

	if (copy == NULL)
		return -2;

	swap(&this->x, &this->y);

	int x = this->x;
	int y = this->y;
	int* a = this->a;

	for(int j = 0; j < y; ++j)
		for (int i = 0; i < x; ++i)
			a[i + j*x] = copy->a[j + i*y];

	if (matrix_dctor(copy) < 0)
		return -3;
	return 0;
}
void * thrRowMul(void* a)
{
	struct thrMulArg* arg = (struct thrMulArg*)a;
	struct mtxdata* data = arg->data;
	int n = arg->n;

	int max = (n == (data->m - 1)) ? (data->result->x)*(data->result->y) : (n+1)*data->work;
	
	if (DUMP_MATRIX) matrix_printf(data->result);
		
	for(int j = n * data->work; j < max ; ++j)
	{
		int result = 0;
		int* a = &data->mtx1->a[data->mtx1->x * (j/data->result->y)];
		int* b = &data->mtx2->a[data->mtx2->x * (j%data->result->y)];
		if (DUMP_MATRIX) printf("%d: ",j);
		for (int i = 0; i < data->mtx1->x; ++i)
		{
			if (DUMP_MATRIX) printf("%d -> ", result);
			result += (a[i]*b[i]);
		}
		if (DUMP_MATRIX) 
		{
			printf("%d\n", result);
			matrix_printf(data->result);
		}

		data->result->a[j] = result;
	}
	return NULL;
}

struct matrix* matrix_lelmul(struct matrix* mtx1, struct matrix* mtx2, int m)
{
	if (!mtx1 || !mtx2) // errno
	{
		errno = EFAULT;
		return NULL;
	}

	if (mtx1->x != mtx2->y) 
	{
		errno = EINVAL;
		return NULL;
	}
	if (matrix_transpose(mtx2) < 0)
		return NULL;
	
	struct matrix* result = matrix_ctor(mtx1->y, mtx2->y, NULL);
	
	pthread_t* thid = (pthread_t*)calloc(m, sizeof(pthread_t));
	if (thid == NULL) return NULL;

	struct mtxdata dt = {mtx1, mtx2, result, m, (result->x)*(result->y)/m};
	struct thrMulArg* arg = (struct thrMulArg*)calloc(m, sizeof(struct thrMulArg));

	for(int i = 0; i < m; ++i)
	{	
		arg[i].data = &dt;
		arg[i].n = i;
		if (errno = pthread_create(&thid[i], NULL, thrRowMul, &arg[i]))
	 		return NULL;
	}

	for(int i = 0; i < m; ++i)
		if (errno = pthread_join(thid[i], NULL))
			return NULL;

	free(thid);
	free(arg);
	
	if (matrix_transpose(mtx2) < 0)
		return NULL;
	return result;
}