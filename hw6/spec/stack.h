#ifndef STACK_H_INCLUDED 
#define STACK_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define ASSERT_OK(type, what)\
{\
int error = type##_OK(what);\
if (error)\
	{\
	printf("OMG! ERROR! ERROR!!11\n" #type " " #what " failed in %s /%s, %d\n",\
	__FILE__, __func__, __LINE__);\
	printf("Error code: %d\n", error);\
	if (error != stack_is_NULL) type ## _DUMP(what, #what, error);\
	abort();\
	}\
}

enum stack_errors
{
	no_error,
	some_sort_of_error,
	stack_is_broken,
	stack_is_NULL,
	arr_is_NULL,
	//	arrPtr_is_NULL,
	arrPtr_is_less_arr,
	wrong_amount,
};
 
#define TYPEofSTACK int 

const unsigned int MAXmemory = 33554432;

typedef struct Stack
{
	TYPEofSTACK* arr; // 
	TYPEofSTACK* arrPtr;
	size_t amount; // number of elements
	size_t size; // number of allocated bytes for stack 
	int state;
} Stack;

inline TYPEofSTACK* ACCESS (void* a)
{
	return (TYPEofSTACK*)a;
}

int Stack_OK(Stack* This)
{
	if (This == NULL) return This->state = 0, stack_is_NULL;
	if (!This->state) return stack_is_broken;
	TYPEofSTACK* arr = (This->arr);
	TYPEofSTACK* arrPtr = (This->arrPtr);
	if (arr == NULL) return This->state = 0, arr_is_NULL;
	//if (arrPtr == NULL) return This->state = 0, arrPtr_is_NULL;
	if (!(arrPtr == NULL))
	{
		if (arrPtr - arr < 0) return This->state = 0, arrPtr_is_less_arr;
		if (This->amount < (int)(arrPtr - arr)) return This->state = 0, wrong_amount;
	}
	return This->state = 1, no_error;
}
bool Stack_DUMP(const Stack* This, const char name[], int error) // error = 0
{
	assert(This);

	FILE* dumpFile = fopen("Stack_dump.txt", "a"); \
		if (dumpFile == NULL) return printf("Can't create a dump file"), 0;
	fprintf(dumpFile, __TIME__ " " __DATE__"\n");
	if (error) fprintf(dumpFile, "OMG! ERROR! ERROR!!11\n\nStack %s failed in %s/%s, %d\n",
		name, __FILE__, __func__, __LINE__);
	fprintf(dumpFile, "********************************************\n");
	fprintf(dumpFile, " arr = %ld\n arrPtr = %ld\n size = %d\n amount = %d\n state = %d\n",
		(intptr_t)This->arr, (intptr_t)(This->arrPtr), (int)This->size, (int)This->amount, This->state);
	if (!error || error == wrong_amount || error == some_sort_of_error)
		for (int i = 0; i*sizeof(TYPEofSTACK) < This->size; ++i)
		{
			fprintf(dumpFile, "arr[%d]: %d", i, (This->arr)[i]);
			if (i < (int)This->amount) fprintf(dumpFile, " *\n");
			else fprintf(dumpFile, "\n");
		}
	fprintf(dumpFile, "********************************************\n\n\n");
	fclose(dumpFile);
	return 1;
}

bool Stack_ctor(Stack* This, size_t buffer) //buffer 32
{
	assert(This);
	bool st = 1;
	//buffer = 32;

	This->amount = 0;
	if (buffer > MAXmemory)
	{
		st = 0;
		buffer = MAXmemory;
	}
	This->size = buffer;
	This->arr = (int*)calloc(buffer, sizeof(TYPEofSTACK));
	This->arrPtr = NULL;
	This->state = true;

	//ASSERT_OK(Stack, This);
	return st;
}
bool Stack_dctor(Stack* This)
{
	ASSERT_OK(Stack, This)
		free(This->arr);
	This->arr = NULL;
	This->arrPtr = NULL;
	This->amount = -1;
	This->size = -1;
	This->state = 0;

	return 1;
}


bool Stack_realloc(Stack* This)
{
	ASSERT_OK(Stack, This);
	This->size *= 2;
	This->arr = (int*)realloc((This->arr), This->size);
	This->arrPtr = (This->arr) + This->amount;
	ASSERT_OK(Stack, This);
}

bool Stack_push(Stack* This, TYPEofSTACK val)
{
	//ASSERT_OK(Stack, This);

	if ((This->amount + 1) * sizeof(TYPEofSTACK) > This->size)
	{
		if (This->size < MAXmemory)
			if (!Stack_realloc(This))
				return 0;
		else return 0;
	}

	else if (This->arrPtr == NULL) This->arrPtr = This->arr;
	else This->arrPtr = (This->arrPtr) + 1;

	*(This->arrPtr) = val;
	++This->amount;

	//ASSERT_OK(Stack, This);
	return 1;

}
bool Stack_pop(Stack* This, TYPEofSTACK* val)
{
	ASSERT_OK(Stack, This);
	assert(val);

	if (This->arrPtr == NULL)
		return 0;

	*val = *(This->arrPtr);
	--This->amount;
	if (This->arrPtr == This->arr)
		This->arrPtr = NULL;
	else This->arrPtr = (This->arrPtr) - 1;

	ASSERT_OK(Stack, This);
	return 1;
}

#endif