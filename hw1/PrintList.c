#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bufrw.h"
#include "nameslib.h"

int main()
{

	int fd1 = open(LIST_NAME, O_RDWR);
	if(fd1 == -1) return perror("open"), 1;

	char* buffer; 
	if (bufread(fd1, &buffer) < 0 )
		{
			perror("ERROR.bufread");
			return 1;
		}

	close(fd1);

	printf("%s", buffer);

	free(buffer);
	remove(LIST_NAME);
	return 0;
}