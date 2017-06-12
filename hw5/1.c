#include <unistd.h>
#include <mylib.h>

int main(int argc, char const *argv[])
{
	pid_t parent = fork();
	if (parent == -1)
		PERRET1(fork);
	else if (parent)
		for(;;);
	else
		for(;;);
	return 0;
}