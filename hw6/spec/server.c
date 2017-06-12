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
#include <string.h>
#include "stack.h"
#include "lib.h"
#include "matrix.h"

//int m_max = 65536; // Stack

char defpathdata[] = "data.txt";
char defpathlog[] = "serverlog.txt";

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

	int fdlog = open(defpathlog, O_RDWR | O_CREAT | O_TRUNC, 0660);
	if (fdlog == -1)
		PERRET1(open);

	int* data1 = (int*)data;
	int* data2 = (int*)(data + num / 2);

	struct matrix mtx1 = {*data1, *(data1 + 1), data1 + 2};
	struct matrix mtx2 = {*data2, *(data2 + 1), data2 + 2};

	if (matrix_transpose(&mtx2) < 0)
		return 1;
	
	struct matrix* result = matrix_ctor(mtx1.y, mtx2.y, NULL);

	int serv = socket(AF_INET, SOCK_STREAM, 0);
	if (serv < 0)
		PERRET1(socket);
	struct sockaddr_in addr = {AF_INET, htons(port), INADDR_ANY};

	if (bind(serv, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		PERRET1(bind);

	bufwriteC(fdlog, "I'm alive");

	if (listen(serv, SOMAXCONN) < 0) 
		PERRET1(listen);

	dprintf(fdlog, "Listening on [0.0.0.0] port: %d \n", port);
	// bufwriteC(fdlog, "Listening on [0.0.0.0] port: ");
	// bufwriteI(fdlog, &port, sizeof(int));
	// bufwriteC(fdlog, "\n");

	int maxfd = serv; 
	fd_set fdsetR, fdsetW;
	Stack sock;
	Stack_ctor(&sock, 32);
	int j = 0; // job

	while(j < result->x * result->y)
	{
		FD_ZERO(&fdsetR);
		FD_SET(serv, &fdsetR);

		for (int i = 0; i < sock.amount; ++i)
		{
			FD_SET(sock.arr[i], &fdsetR);
			FD_SET(sock.arr[i], &fdsetW);
		}

		int* a = &mtx1.a[mtx1.x * (j/result->y)];
		int* b = &mtx2.a[mtx2.x * (j%result->y)];
	
		if (select(maxfd + 1, &fdsetR, &fdsetW, NULL, NULL) < 0)
			PERRET1(select);

		if (FD_ISSET(serv, &fdsetR))
		{
			int acc_ret = accept(serv, NULL, NULL);
			if (acc_ret < 0)
				PERRET1(accept);
			dprintf(fdlog, "%d connected\n", acc_ret);

			Stack_push(&sock, acc_ret); //valid matrix
			if (acc_ret > maxfd)
				maxfd = acc_ret;
		} //!

		for (int i = 0; i < sock.amount && j < result->y * result->x; ++i)
		{

			if (FD_ISSET(sock.arr[i], &fdsetW))
			{
				int* msg = calloc( 2 + mtx1.x * 2, sizeof(int));
				int k = 0;
				msg[k++] = DOWORK;
				msg[k++] = mtx1.x;
				
				memcpy(&msg[k], mtx1.a, mtx1.x * sizeof(int));
				k += mtx1.x;
				memcpy(&msg[k], mtx2.a, mtx2.x * sizeof(int));
				send(sock.arr[i], msg, k * sizeof(int), 0);

				dprintf(fdlog, "Message sent to %d %c\n", sock.arr[i], (char)msg[0]);
				for (int j = 1; j < k; ++ j)
					dprintf(fdlog, "%d", msg[i]);
				bufwriteC(fdlog, "\n");

				++j;
			}

			if (FD_ISSET(sock.arr[i], &fdsetR))
			{
				int msg[MAX_RCV];
				memset(msg, 0, MAX_RCV);

				recv(sock.arr[i], &msg, MAX_RCV, 0);


				if (msg[0] == WORKDONE)
				{

					dprintf(fdlog, "Message received %c : %d", (char)msg[0], msg[1]);
					//printf("%d\n", msg[1]);
					result->a[j] = msg[1];
				}
			}
		}
	}

	//matrix_printf(result);

	return 0;
}