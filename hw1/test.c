#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	int fd1 = open("text.txt", O_WRONLY | O_CREAT | O_EXCL, 0666);
	char str[] = "1234567890";
	int n = atoi(argv[1]);
	int l = write(fd1, str, strlen(str) + n);
	printf("%d\n", l);
	perror("write");
	remove("text.txt");
	return 0;
}