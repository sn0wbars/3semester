#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
	fprintf(stderr, "Too few arguments\n");
	return 1;
    }

    char* dir = argv[1];
    char* res = argv[2];

    uid_t UID = getuid();
    gid_t GID = getgid();
    pid_t PID = getpid();
    pid_t PPID = getppid();
    
    printf("Process and user information:\n UID: %d\n GID: %d\n PID: %d\n PPID: %d\n\n", UID, GID, PID, PPID);

    pid_t parent = fork(); 
    if (parent == -1)
    {
	fprintf(stderr, "Can't create child process\n");
	return 1;
    }
    else if (parent)
    {
	//sleep(1);

	char* path = calloc(strlen(argv[1]) + strlen(argv[2]) + 23, sizeof(*argv[1]));
	if (!path) 
    	{
	    fprintf(stderr, "Can't allocate memory\n");
	    return 1;
    	}
	sprintf(path, "cat '%s'/*.txt > ./'%s'", dir, res);
	
	if (execlp("bash", "bash", "-c", path, NULL) == -1)
	{
	    fprintf(stderr, "Can't execute bash");
	    return 1;
	}    
    }
    else  
    {
	printf("List of all files in \"%s\":\n", argv[1]);
	if(execlp("ls", "ls", argv[1], NULL) == -1);
	{
	    fprintf(stderr, "Can't execute ls");
	    return 1;
	}
    }

    return 0;
}
    
