#ifndef _ARRAY_H
#define _ARRAY_H

#include <stdlib.h>
#include <stdbool.h>
#define TYPE int

typedef struct
{	
	size_t size; // max_elements
	size_t num; // 
	int (*compar)(const void*, const void*);
	TYPE* array;
} OrdArray;

typedef struct
{
	OrdArray* oarray;
	int index;
} Iterator_OrdArr; 

#define Iterator Iterator_OrdArr //Iterator_OrdArr shortage

Iterator Iterator_ctor(OrdArray* arr);// synonim for OrdArray.begin()
int Iterator_RIP(Iterator* it);
Iterator Iterator_CopyCtor(Iterator* it);
int Iterator_iterate(Iterator* it);	
int Iterator_distance(Iterator first, Iterator last);
TYPE Iterator_get(Iterator* it);
TYPE* Iterator_getp(Iterator* it);
bool Iterator_hasNext(Iterator* it);

int OrdArray_RIP(OrdArray* ths);
int OrdArray_ctor(OrdArray* ths, size_t size, int (*compar)(const void*, const void*));
Iterator OrdArray_begin(OrdArray* ths);
Iterator OrdArray_end(OrdArray* ths);
int OrdArray_dctor(OrdArray* ths);
int OrdArray_shiftRight(OrdArray* ths, int pos);
int OrdArray_shiftLeft(OrdArray* ths, int pos);
TYPE OrdArray_get(OrdArray* ths, int n);
TYPE* OrdArray_getp(OrdArray* ths, int n);
int OrdArray_add(OrdArray* ths, TYPE elem);
int OrdArray_removeElem(OrdArray* ths, TYPE elem);
int OrdArray_removeIndx(OrdArray* ths, int index);
void OrdArray_dump(OrdArray* ths);
void OrdArray_print_int(OrdArray* ths);
#endif