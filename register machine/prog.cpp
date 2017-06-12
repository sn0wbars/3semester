#include <stdio.h>
#include <string.h>
#include <stdio.h>

#define J(x) #x"0", #x"1", #x"2", #x"3", #x"4", #x"5", #x"9"
//#define J(x) #x"0", #x"1", #x"2", #x"3", #x"4", #x"5", #x"6", #x"8"
// #define J(x) #x"0", #x"1", #x"3", #x"3", #x"4", #x"5", #x"6", #x"7, #x"9"     

const int MAX_ITER = 300;
const char fname[] = "list.txt";

//char cmd[][3] = {"i0", "i1", "i2", "i3", "i4", J(a), J(b), J(c), J(d), J(e)};
const char cmd[][3] = {"i0", "i1", "i2", "i3", "i4", J(a), J(b), J(c), J(d), J(e)};
const char b0 = 12; //13
const char c0 = 19; //21
const char d0 = 26; //29


int exec(char* prog, int x, int y);
inline void cpu_clear();
void dump_cpu();
int gen();
void test();

int main()
{
	gen();
	test();
	//char test[] = "c4b7i0i3b2i1d5b0";
	//char prog[] = "i0d0i3b0c9a2";
	//int res = exec(test, 3 ,5);
	//printf("%d", res);
	return 0;
}

int exec(char* prog, int x, int y)
{
	int reg[5] = {};
	reg[1] = x;
	reg[2] = y;
	int i = 0;
	int n_cmd = strlen(prog);
	for(int n = 0; prog[i] && i < n_cmd; ++n)
	{
		//dump_cpu(reg);
		//printf("%d: %c%c\n", i, prog[i], prog[i+1]);
		
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

int gen()
{
	FILE* list = fopen(fname, "w");
	if (list == NULL) return -1;

	char prog[16] = {};
	int i[8];
	for (i[0] = 0; i[0] < (sizeof cmd) / 3; ++ i[0])
	{
		for (i[1] = 0; i[1] < (sizeof cmd) / 3; ++ i[1])
			for (i[2] = 0; i[2] < (sizeof cmd) / 3; ++ i[2])
				for (i[3] = 0; i[3] < (sizeof cmd) / 3; ++ i[3])
					for (i[4] = 0; i[4] < (sizeof cmd) / 3; ++ i[4])
						for (i[5] = 0; i[5] < (sizeof cmd) / 3; ++ i[5])
//							for (i[6] = 0; i[6] < (sizeof cmd) / 3; ++ i[6])
//								for (i[7] = 0; i[7] < (sizeof cmd) / 3; ++ i[7])
								{
									bool fl1 = 0;
									bool fl2 = 0;
									bool fl3 = 0;
									int k = 0;
									for(; k < 6; ++k)
									{
										if (i[k] == 0) 
											fl1 = 1;
										if (i[k] >= b0 && i[k] < c0) 
											fl2 = 1;
										if (i[k] >= c0 && i[k] < d0) 
											fl3 = 1;
									}
									if ( !(fl1 && fl2 && fl3) ) 
										continue;
									
									//strcat(prog, cmd[i[7]]);
									//strcat(prog, cmd[i[6]]);
									strcat(prog, cmd[i[5]]);
									strcat(prog, cmd[i[4]]);
									strcat(prog, cmd[i[3]]);
									strcat(prog, cmd[i[2]]);
									strcat(prog, cmd[i[1]]);
									strcat(prog, cmd[i[0]]);
									if (exec(prog, 0, 1) == 0 && exec(prog, 2, 0) == 0 && exec(prog, 1, 0) == 0 && exec(prog, 1, 2) == 2 && exec(prog, 3, 3) == 9)
										{
											printf("%s\n", prog);
											fprintf(list, "%s\n", prog);
										}
										//printf("%d\n", sizeof cmd );
									memset(prog, 0, 16);
								}
		printf("!\n");	
	}
	fclose(list);
}

void test()
{
	FILE* list = fopen(fname, "r");
	char prog[16] = {};
	while (fscanf(list, "%s", prog) != -1)
	{
		if (exec(prog, 1, 0) == 0 && exec(prog, 0, 1) == 0 && exec(prog, 1, 2) == 2 && exec(prog, 2, 1) == 2 && exec(prog, 2, 2) == 4)
			printf("%s\n", prog);
	}
}

void cpu_clear(int reg[5])
{
	for(int i = 0; i < 5; reg[i++] = 0);
}

void dump_cpu(int reg[5])
{
	printf("r0: %d\nr1: %d\nr2: %d\nr3: %d\nr4: %d\n", reg[0], reg[1], reg[2], reg[3], reg[4]);
}