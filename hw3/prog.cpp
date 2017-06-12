#include <stdio.h>

int main()
{
	char* prog = "i0i0i0i0i0";
	int res = exec(prog, 0 ,0);
}

int exec(char* programm, int x, int y)
{
	int reg[5] = {};
	reg[1] = x;
	reg[2] = y;
	int k = 0;
	for(int i = 0; prog[i] && i <= 16)
		if (programm[i++] == 'i') //
			++reg[programm[i++] - '0'];
		
		else if ('a' <= prog[i++] <= 'e')
		{
			int* reg = &reg[prog[i++] - 'a'];
			if (*reg) 
				--*reg;
			else 
			{
				++i;
				continue;
			}

			i = prog[i++]*2;
			if (i > 16)
				break;	
		}
		else return (-1);
	}
	return (reg[0]);

