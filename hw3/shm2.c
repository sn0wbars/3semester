#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <bufrw.h>
#include <mylib.h>
#include <stdlib.h>
#include <sys/types.h>	
#include <string.h>

int main()
{
	key_t key = ftok(getenv("HOME"), 0);
	if (key == -1)
		PERRET1(ftok);
	int id = shmget(key, 0, 0);
	if (id == -1)
		PERRET1(shmget);

	char* shmem = shmat(id, NULL, SHM_RDONLY);
	if (shmem == (char*)-1)
		PERRET1(shmat);
	
	if (strlen(shmem) == strlen("  PID TTY      STAT   TIME COMMAND\n")) 
		printf("Not existing id\n");
	else
		printf("%s", shmem);

	if (shmctl(id, IPC_RMID, NULL) == -1)
		PERRET1(shmctl);

	if (shmdt(shmem) == -1)
		PERRET1(shmdt);

	return 0;
}