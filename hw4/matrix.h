#ifndef _MATR_H_
#define _MATR_H_x

struct matrix
{
	int y;
	int x;
	int* a;
};

void* swap(int *a, int *b);
int matrix_rand(struct matrix* this, int max);
int matrix_fprint(int fd, struct matrix* this);
int matrix_printf(struct matrix* this); // not a pointer
struct matrix* matrix_ctor(int y, int x, int* a);
struct matrix* matrix_copy_ctor(struct matrix* this);
int matrix_dctor(struct matrix* this);
int matrix_transpose(struct matrix* this);
int matrix_check(struct matrix* this);
void * thrRowMul(void* a);
struct matrix* matrix_lelmul(struct matrix* mtx1, struct matrix* mtx2, int m);

#endif