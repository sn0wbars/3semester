#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bufrw.h"
#include "nameslib.h"

const char DEFAULT_PATH[] = "./";
const char string[] = "List of files:\n";

void print_help();

int main(int argc, char* argv[])
{

	char *dir = (char*)DEFAULT_PATH;

	if (argc > 2) printf("Too many arguments");
	if (argc == 2)
		{
			if(strcmp(argv[1], "--help") == 0)
			{
				print_help();
				return 1;
			}
			dir = argv[1];
		}

	umask(0);
	int fd1 = open(LIST_NAME, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if(fd1 == -1) return perror("open"), 1;

	printf("!!!!");

	if (bufwrite(fd1, (char*)string) < 0) 
	{
		perror("ERROR.bufwrite");
		return 1;
	}

	close(fd1);

	char* shell = getenv("SHELL");

	size_t strsize = strlen(dir) + strlen(LIST_NAME) + strlen("ls ") + strlen(" >> ");
	char str[strsize];
	
//	printf("!!!!");
//	printf("%s!!!!!!!!", str);
	strcpy(str, "ls ");
	strcat(str, dir);
	strcat(str, " >> ");
	strcat(str, LIST_NAME);

	if (execlp(shell, shell, "-c" , str, NULL) == -1) 
		return perror("execlp"), 1; //можно использовать cat вместо open/bufwrite

	return 0;

}


void print_help()
{
		printf("This programm prints names of all files in directory\n\
Keys:\n\
--help   Print help\n\
There should be zero or one parameter\n\
Parametres:\n\
1. directory //(default: current directory)\n");

};