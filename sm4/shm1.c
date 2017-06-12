#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>	
#include <string.h>

const size_t size = 100;

int main()
{
	key_t key = ftok (__FILE__, 0);
	umask(0);
	int id = shmget(key, size, IPC_CREAT | 00777);
	char* p_memes = (char*) shmat(id, NULL, 0);

	printf("%s\n", p_memes);
	printf("%d\n", key);
	strcpy(p_memes, "Holy cow!");


	shmdt(p_memes);

	return 0;
}
