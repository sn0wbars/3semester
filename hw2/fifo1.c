#define _GNU_SOURCE 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "bufrw.h"
#include "nameslib.h"

const char DEFAULT_PATH[] = "./";

int main(int argc, char* argv[])
{
	if (argc > 2) printf("Too many arguments");
	char *dir = (argc == 2) ? argv[1] : (char*)DEFAULT_PATH;

    int pipefd[2] = {};
    if (pipe2(pipefd, O_CLOEXEC) < 0)
    	return perror("ERROR.pipe"), 1; //syntax sugar?
    
    int parent = fork();
    if (parent == -1) 
    	return perror("ERROR.fork"), 1;	
   
    if (parent)
    {
		close(pipefd[0]);
		if (dup2(pipefd[1], 1) < 0)
			return fprintf(stderr,"ERROR.dup2"), 1;
		if (execlp("ls", "ls", dir, NULL) < 0) 
			return fprintf(stderr,"ERROR.execlp"), 1;//
    }
    
    if (!parent)
    {
		close(pipefd[1]);
		char* list = NULL;
		if (bufread(pipefd[0], &list) < 0)
			return perror("ERROR.bufread"), 1;
		close(pipefd[0]);

		umask(0);
		if (mkfifo(FIFOPATH, 0666) < 0) 
			return perror("ERROR.mkfifo"), 1;

		int fd = open(FIFOPATH, O_WRONLY);
		if (fd == -1) 
			return perror("ERROR.open"), 1;

		if (bufwrite(fd, list) 	< 0)
			return perror("ERROR.write"), 1;
		
		free(list);
		close(fd);
    }
    return 0;
}