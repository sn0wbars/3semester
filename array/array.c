#include <stdio.h> 
#include <mylib.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

const int DUMP = 0;

Iterator Iterator_ctor(OrdArray* arr)// synonim for OrdArray
{
	Iterator it;
	it.oarray = arr;
	it.index = (arr != NULL)? 0 : -1;
	return it;
}	

int Iterator_RIP(Iterator* it)
{
	//printf("asdasd%d\n", it->index);
	if (it == NULL || it->oarray == NULL || it->index < 0 || it->index >= it->oarray->num)
		return 1;
	else 
		return 0;
}

Iterator Iterator_CopyCtor(Iterator* it)
{
	Iterator copy;
	copy.oarray = it->oarray;
	copy.index = it->index;
	return copy;
}	

int Iterator_iterate(Iterator* it)
{
	if (Iterator_RIP(it))
		return 1;
	++(it->index);
	return 0;
}

Iterator Iterator_next(Iterator* it)
{
	Iterator it_next = Iterator_CopyCtor(it);
	Iterator_iterate(&it_next);
	return it_next;
}

int Iterator_distance(Iterator first, Iterator last)
{
	return (last.index - first.index);
}

int OrdArray_RIP(OrdArray* ths)
{
	if (ths == NULL || ths->size < 0 || ths->num < 0 || ths->compar == NULL || ths->num > ths->size) 
		return 1; 
	else 
		return 0;
}

int OrdArray_ctor(OrdArray* ths, size_t size, int (*compar)(const void*, const void*))
{
	if(ths == NULL) return 1;
	
	ths->compar = compar;
	ths->size = size;
	ths->num = 0;
	ths->array = (TYPE*)calloc(size, sizeof(TYPE));
	if (ths->array == NULL) 
	 	return 1;
	if (OrdArray_RIP(ths))
		return 1;
	else
		return 0;
}

Iterator OrdArray_begin(OrdArray* ths)
{
	return Iterator_ctor(ths); 
}

Iterator OrdArray_end(OrdArray* ths)
{
	Iterator it = Iterator_ctor(ths);
	it.index = ths->num - 1;
	return it;
}

int OrdArray_dctor(OrdArray* ths)
{
	if (ths == NULL || OrdArray_RIP(ths))
		return 1;
	ths->compar = NULL;
	ths->size = -1;
	ths->num = -1;

	free(ths->array);
	ths->array = NULL;

	return 0;
}

int OrdArray_shiftRight(OrdArray* ths, int pos)
{
	if (ths == NULL || OrdArray_RIP(ths))
		return 1;

	if (pos > ths->size)
		return 0;

	if (ths->num == ths->size)
		return 2; //overflow

	memmove(&ths->array[pos+1], &ths->array[pos], (ths->size - pos)*sizeof(TYPE));
	return 0;
}

int OrdArray_shiftLeft(OrdArray* ths, int pos)
{
	if (ths == NULL || OrdArray_RIP(ths))
		return 1;

	if (pos > ths->size)
		return 0;

	if (ths->num == ths->size)
		return 2; //overflow

	memmove(&ths->array[pos-1], &ths->array[pos], (ths->size - pos)*sizeof(TYPE));
	return 0;
}

int 

TYPE Iterator_get(Iterator* it)
{
	if (it == NULL || Iterator_RIP(it))
		return (TYPE)0;
	return it->oarray->array[it->index];
}

TYPE* Iterator_getp(Iterator* it)
{
	if (it == NULL)
		return NULL;
	return &(it->oarray->array[it->index]);
}

TYPE OrdArray_get(OrdArray* ths, int n)
{
	if (ths == NULL || OrdArray_RIP(ths) || n >= ths->num)
		return (TYPE)0;
	return ths->array[n];
}

TYPE* OrdArray_getp(OrdArray* ths, int n)
{
	if (ths == NULL || OrdArray_RIP(ths) || n >= ths->num)
		return NULL;
	return &ths->array[n];
}

bool Iterator_hasNext(Iterator* it)
{
	if (it == NULL || Iterator_RIP(it) || it->index >= it->oarray->num - 1)
		return 0;
	else return 1;
}

int OrdArray_add(OrdArray* ths, TYPE elem)
{
	if (ths == NULL || OrdArray_RIP(ths))
		return 1;

	if (ths->num == ths->size)
		return 2; //overflow

	Iterator i = Iterator_ctor(ths);
	if (Iterator_hasNext(&i))
	for(i; ths->compar((void*)&elem, (void*)Iterator_getp(&i)) > 0 && !Iterator_iterate(&i););
	OrdArray_shiftRight(ths, i.index);
	ths->array[i.index] = elem;
	if (DUMP) 
		printf("index: %d\nelem: %d\n", i.index, elem);
	++(ths->num);
	
	return 0;
}

// int OrdArray_merge(OrdArray* ths, OrdArray* sec)
// {
// 	if (ths == NULL || sec == NULL || OrdArray_RIP(ths) || OrdArray_RIP(sec))
// 		return 1;
// 	Iterator i = OrdArray_begin(ths);
// 	Iterator j = OrdArray_begin(sec);

	
// }

int OrdArray_removeElem(OrdArray* ths, TYPE elem)
{
	if (ths == NULL || OrdArray_RIP(ths))
		return 1;
	if (ths->num == 0)
		return 0;
	Iterator iter = Iterator_ctor(ths);
	while( (Iterator_get(&iter) != elem) && !Iterator_iterate(&iter) );
	if (Iterator_RIP(&iter))
		return 1;
	--ths->num;
	OrdArray_shiftLeft(ths, iter.index+1);
	return 0;
}

int OrdArray_removeIndx(OrdArray* ths, int index)
{
	if (ths == NULL || OrdArray_RIP(ths))
		return 1;
	if (index >= ths->num)
		return 2;

	--ths->num;
	OrdArray_shiftLeft(ths, index + 1);
	return 0;
}

void OrdArray_dump(OrdArray* ths)
{
	printf("Size: %d\nNum: %d\nCompar: %p\n Array: %p\n", (int)ths->size, (int)ths->num, (void*)ths->compar, (void*)ths->array);
}

void OrdArray_print_int(OrdArray* ths)
{
	printf("-----------------\n");
	for (int i = 0; i < ths->num; ++i)
		printf("%d: %d\n", i, ths->array[i]);
	printf("-----------------\n");
}
