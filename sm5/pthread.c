#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>

void* my_thread(void* arg)
{
	printf("%d\n", (*(int*)arg)++);
	return arg;
}

int main()
{
	pthread_t this = pthread_self();

	printf("%lx\n", this);
	
	pthread_t new_thid[10];
	
	int i;
	for(i = 0; i < 10; i++)
	{
		if ((errno = pthread_create(&new_thid[i], NULL, my_thread, &i)))
			return perror("ERROR"), 1;
	}
	int** b = malloc(sizeof(int*));

	for(i; i < 10; ++i)
	{
		pthread_join(new_thid[i], (void**)b);
		printf("a = %d\n", **b);
	}

	return 0;
}
