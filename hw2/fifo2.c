#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "bufrw.h"
#include "nameslib.h"

int main()
{
    int fd = open(FIFOPATH, O_RDONLY | O_CLOEXEC);
   		if (fd == -1) 
   			return perror("ERROR.open"), 1;
 	
 	char* list = NULL;   
    if (bufread(fd, &list) < 0) 
    	return perror("ERROR.read"), 1;
    
    printf("%s", list);
    
    if (execlp("rm", "rm", FIFOPATH, NULL) < 0)
    	return perror("ERROR.rm"), 1;

    return 0;
}