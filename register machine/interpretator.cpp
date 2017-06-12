#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_ITER = 500;

inline void dump_cpu(int reg[5]);
int exec(char* prog, int reg[5]);

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("not enough arguments\n");
		return -1;
	}
	char* prog = argv[1];

	int reg[5] = {};
	for(int i = 0; i < 5; ++i)
		reg[i] = (*(argv[2] + i) - '0');

	printf( "%d\n", exec(prog, reg) );
	return 0;
}

void dump_cpu(int reg[5])
{
	printf("r0: %d\nr1: %d\nr2: %d\nr3: %d\nr4: %d\n", reg[0], reg[1], reg[2], reg[3], reg[4]);
}

int exec(char* prog, int reg[5])
{
	int i = 0;
	int n_cmd = strlen(prog);
	for(int n = 0; prog[i] && i < n_cmd; ++n)
	{
		dump_cpu(reg);
		printf("%d: %c%c\n", i, prog[i], prog[i+1]);
		
		if (n >= MAX_ITER) return -2;
		if (prog[i] == 'i') //
			{
				++reg[prog[++i] - '0'];
				++i;
			}
		
		else if ('a' <= prog[i] && prog[i] <= 'e')
		{
			//printf("%d\n", reg[prog[i] - 'a']);
			if (reg[prog[i] - 'a'])
				--(reg[prog[i++] - 'a']);
			else 
			{	
				i+=2;
				continue;
			}
			i = (prog[i] - '0')*2;
			if (i > n_cmd)
				break;	
		}
		else 
			return -1;
	}  
	return reg[0];
}