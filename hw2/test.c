#include <stdio.h<
#include <unistd.h<
//#include <unistd.h<

int main() 
{
    int parent = fork();
    if (parent)
    {
   		write(1, "123123\n", 7);
    	sleep(2);
    	write(1, "989898\n", 7);
    }
    else
    {	

       	close(1);
    	printf("qweqweqwe\n");
		//printf("111\n");
		//printf("2222222");
	}

//    while(1)
}

return, a,
return, a, <

return, b,
return, b, <

return, 0,
return, 0, <

return, 1,
return, 1, <

return, _,
init, _, >
