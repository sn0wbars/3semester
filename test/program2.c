#include "bufreadwrite.h"
#include <sys/shm.h>
#include <sys/ipc.h>

int main()
{
	char* shmem = NULL;
	int shmid;
	char file[] = "program1.c";
	key_t key;

	if ((key = ftok("/home/sn0wbars/a.txt", 2)) < 0)
		{
			perror("can't generate key");
			exit(-1);
		}
	printf("%lld\n", &key);

	if ((shmid = shmget(key, 0, SHM_RDONLY) < 0))
	{
		perror("can't find shared memory");
		exit(-1);
	}

	printf("%d\n", shmid);
	if ((shmem = (char*)shmat(shmid, NULL, 0)) == (char*)(-1))
	{			
		perror("can't attach shared memory");
		exit(-1);
	}

	printf("%s\n", shmem);

	return 0;
}


