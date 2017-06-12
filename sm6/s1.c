#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <bufrw.h>
#include <mylib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
//#include <mylib.h>	
#include <string.h>

int main()
{
	key_t key = ftok(getenv("HOME"), 0);

	int shmid = shmget(key, 2 * sizeof(char), IPC_CREAT | 0600);
	char* str = shmat(shmid, NULL, 0);

	int semid = semget(key, 1, IPC_CREAT | 0600);
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);

	strcat(str, "123");
	printf("%s", str);

	return 0;
}