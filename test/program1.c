#include "bufreadwrite.h"
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char const *argv[])
{
	const char* pid = argv[1]; 
	char* argps = NULL;
	char* shmem = NULL;
	int pipefd[2] = {};
	int shmid = 0;
	char file[] = "/home/sn0wbars/a.txt";
	key_t key = 0;

	if (argc < 2)
	{
		printf("There were not enough arguments, please enter a pid.\n");
		return 0;
	}

	if (pipe(pipefd) < 0)
    {
   		perror("error during call pipe"); 
   		exit(EXIT_FAILURE);
   	}


    pid_t p = fork();

    if (p < 0)
    {
    	perror("error during fork");
		exit(-1);
    }


    if (p)
    {
    	if (close(pipefd[0]) != 0)
			perror("error during close pipefd[0]");

		if (dup2(pipefd[1], 1) < 0)
		{
			perror("error during dup2");
			exit(-1);
		}

		int lfcall = strlen("ps -p ") + strlen(pid) + 1;

		argps = calloc(lfcall, sizeof(char));

		if (argps == NULL)
			perror("error during allocation of memory");

		strcat(argps, "ps -p ");
		strcat(argps, pid);

		execl(getenv("SHELL"), getenv("SHELL"),  "-c", argps, NULL);
		perror("error during execl with ps");

    }
    else
    {
    	if (close(pipefd[1]) != 0)
			perror("error during close pipefd[1]");

		char* data = NULL;

		data = buf_read(pipefd[0]);

		if (data == NULL)
		{
			perror("error during buf_read");
			exit(-1);
		}

		if (close(pipefd[0]) != 0)
			perror("error during close pipefd[0]");

		key = ftok(getenv("HOME"), 0);
		if (key == -1)
		{
			//printf("%lld\n", key);
			perror("can't generate key");
			exit(-1);
		}
		if ((shmid = shmget(key, sizeof data, 0666|IPC_CREAT|IPC_EXCL)) < 0)
		{
			if (errno != EEXIST)
			{
				perror("unknown error during allocation of shared memory");
				exit(-1);
			}
			else
			{
				perror("shared memory alredy exists");
				exit(-1);
			}
		}

		if ((shmem = (char*)shmat(shmid, NULL, 0)) == (char*)(-1))
		{
			perror("can't attach shared memory");
			exit(-1);
		}

		strcpy(shmem, data);

		printf("%s\n", shmem);
    }

    //shmdt(shmem);

    //shmctl(shmid,  IPC_RMID, NULL);
	return 0;
}