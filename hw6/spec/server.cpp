#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <mylib.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <bufrw.h>
#include "stack.h"
#include "lib.h"
#include "matrix.h"

//int m_max = 65536; // Stack

char defpathdata[] = "data.txt";

int main(int argc, char const *argv[])
{
	const char* fpathdata = (argc < 2)? defpathdata: argv[1];
	const char* fpathlog = (argc < 3)? defpathlog: argv[2];

	int fd1 = open(fpathdata, O_RDONLY);
	if (fd1 == -1)
		PERRET1(open);

	char* data = NULL;
	
	int num = bufread(fd1, &data);
	if (num == -1)
		PERRET1(bufread);

	if (close(fd1) < 0)
		PERRET1(close);

	int* data1 = (int*)data;
	int* data2 = (int*)(data + num / 2);

	struct matrix matrix1 = {*data1, *(data1 + 1), data1 + 2};
	struct matrix matrix2 = {*data2, *(data2 + 1), data2 + 2};

	if (matrix_transpose(mtx2) < 0)
		return NULL;
	
	struct matrix* result = matrix_ctor(mtx1->y, mtx2->y, NULL);

	int serv = socket(AF_INET, SOCK_STREAM, 0);
	if (serv < 0)
		PERRET1(socket);
	struct sockaddr_in addr = {AF_INET, htons(4444), INADDR_ANY};

	if (bind(serv, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		PERRET1(bind);

	listen(serv, SOMAXCONN);
	int maxfd = serv; 
	fd_set fdsetR, fdsetW;
	FD_ZERO(&fdsetR);
	FD_SET(serv, &fdsetR);

	Stack* sock;
	Stack_ctor(sock, 32);
	int j = 0; // job

	while(j < result->x * result->y)
	{
		FD_ZERO(&fdsetR);
		for (int i = 0; i < sock->amount; ++i)
			FD_SET(sock->arr[i], &fdsetR);

		int* a = mtx1->a[mtx1->x] * (j/result->y)];
		int* b = mtx2->a[mtx2->x] * (j%result->y)];
	
		if (select(maxfd, fdsetR, fdsetW, NULL, NULL) < 0)
			PERRET1(select);

		if (FD_ISSET(serv, &fdsetR))
		{
			int acc_ret = accept(serv, NULL, NULL);
			if (acc_ret < 0)
				PERRET1(accept);
			Stack_push(sock, acc_ret); //valid matrix
		} //!

		for (int i = 0; i < sock->amount && j < result->y * result->x; ++i)
		{

			if (FD_ISSET(sock->arr[i], &fdsetW)
			{
				int* msg = calloc( 2 + m*mtx1->x * 2, sizeof(int));
				int k = 0;
				msg[k++] = DOWORK;
				msg[k++] = mtx->x;
				
				memcpy(msg[k], mtx1->a, mtx1->x * sizeof(int))
				k += mtx1->x;
				memcpy(msg[k], mtx2->a, mtx2->x * sizeof(int))
				send(sock->arr[i], msg, k * sizeof(int), 0);

				++j;
			}

			if (FD_ISSET(sock->arr[i], &fdsetR))
			{
				int msg[MAX_RCV];
				memset(msg, 0, MAX_RCV);

				recv(sock->arr[i], &msg, MAX_RCV, 0);
				if (msg[0] == WORKDONE)
					result->a[j] = msg[1];
			}
		}
	}

	return 0;
}