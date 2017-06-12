#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <unistd.h>
#include <bufrw.h>
#include <mylib.h>

const size_t size = 100;

int main(int argc, char* argv[])
{
	char* pid = (argc >= 2) ? argv[1] : NULL; 

	int pipefd[2] = {};
	if (pipe(pipefd) == -1)
		PERRET1(pipe);

	pid_t parent = fork();
	if (parent == -1)
		PERRET1(fork);
	if (!parent)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], 1) == -1)
			return fprintf(stderr,"ERROR.dup2"), 1;
		if (execlp("ps", "ps", pid, NULL) == -1) 
			return fprintf(stderr,"ERROR.ps"), 1;
	}

	if(parent)
	{
		close(pipefd[1]);
		char * pid_info = 0;
		
		if (bufread(pipefd[0], &pid_info) == -1)
			PERRET1(bufwrite);

		key_t key = ftok (getenv("HOME"), 0);
		if (key == -1)
			PERRET1(ftok);
		umask(0);
		int id = shmget(key, strlen(pid_info), IPC_CREAT | IPC_EXCL | 0600);
		if (id == -1)
			PERRET1(shmget);

		char* shmem = shmat(id, NULL, 0);
		if (shmem == (char*)-1)
			PERRET1(shmat);

		strcpy(shmem, pid_info);

		if (shmdt(shmem) == -1)
			PERRET1(shmdt);
	}

	return 0;
}	