#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <mylib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bufrw.h>
#include "matrix.h"
#include "lib.h"

char def_addr[] = "127.0.0.1"; 
char defpathlog[] = "log.txt";

int main(int argc, char const *argv[])
{
	int fd1 = open(defpathlog, O_WRONLY | O_CREAT | O_TRUNC, 0660);
	if (fd1 == -1)
		PERRET1(open);

	bufwriteC(fd1, "I'm alive\n");

	const char* ip_addr = (argc < 2)? def_addr : argv[1];
	const char* fpath = (argc < 3)? defpathlog : argv[2];

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
		PERRET1(socket);

	struct sockaddr_in addr = {AF_INET, htons(port)};
	if (inet_pton(AF_INET, ip_addr, (void*)&addr.sin_addr) <= 0)
		PERRET1(inet_ntop);

	//inet_ntop - совместимость с IPv4 и IPv6/ 255.255
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		PERRET1(connect);

	bufwriteC(fd1, "Connected to server: ");
	bufwriteC(fd1, def_addr);
	bufwriteC(fd1, "\n");

	int msg[MAX_RCV];
	memset(msg, 0, MAX_RCV);

	while(1)
	{
		int num_recv = recv(sock, &msg, MAX_RCV, 0);
		bufwriteC(fd1, "Received message");

		if (num_recv < 0)
			PERRET1(recv);

		// else if (num_recv == 0)
		// {
		// 	printf("smth happened with recv\n");
		// 	return 1;
		// }

		else if (msg[0] == DOWORK)
		{	

			int * a = &msg[2];
			int * b = &msg[2] + msg[1];

			int result = 0;
			for (int i = 0; i < msg[1]; ++i)
			{
				result += a[i]*b[i];
			}
			
			int response[2] = {WORKDONE, result}; 
			int num_sent = send(sock, response, sizeof(response), 0) < 0;
			bufwriteC(fd1, "Sent message[WOKDONE]: ");
			bufwriteI(fd1, (char*)response, sizeof(int) * sizeof(response));
			
			if (num_sent < 0) 
				PERRET1(send);
			// else if (num_sent == 0)
			// 	{
			// 		printf("smth happened with send\n");
			// 		return 1;
			// 	}
		}

		else if (msg[0] == BYE)
		{
			bufwriteC(fd1, "\nBye\n");
			close(sock);
			return 0;
		}
	}
	return 0;
}