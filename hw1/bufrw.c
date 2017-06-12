#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <bufrw.h>
//todo: doxygen

const size_t BUF_SIZE = 4096;

ssize_t bufread(int fd, char ** p_buf) 
{
	if(p_buf == NULL)
	{
		errno = EFAULT; 
		return -1;
	}

	size_t bufsize = BUF_SIZE;
	*p_buf = calloc(bufsize, sizeof(char));

	if (*p_buf == NULL)
		return -2;
	
	ssize_t len = 0;
	ssize_t snum = 0;
	while(snum = read(fd, *p_buf + len, BUF_SIZE))
	{
		if(snum == -1) 
			return len;

		len += snum; 
		if (len >= bufsize - BUF_SIZE)
		{
			bufsize *= 2;
			*p_buf = realloc(*p_buf, bufsize);
			if (!(*p_buf)) 
				return -2;
		}
	}
	return len;
}

ssize_t bufwriteC(int fd, char* buf)
{
	size_t strLen = strlen(buf);
	if(buf == NULL) 
		{
			errno == EFAULT;
			return -1;
		}

	ssize_t nleft = strLen;
	size_t bufsize = BUF_SIZE;
	ssize_t num = 0; 

	while(nleft)
	{
		if (nleft < bufsize) 
			bufsize = nleft;
		if(!bufsize)
			return -3;
		if( (num = write(fd, buf + (strLen - nleft), bufsize)) < 0)
			return -1; //Зацикливание? <=0
		nleft -= num;
	}
	return (strLen - nleft);
}

ssize_t bufwriteI(int fd, char* buf, int strLen)
{
	if(buf == NULL) 
		{
			errno == EFAULT;
			return -1;
		}

	ssize_t nleft = strLen;
	size_t bufsize = BUF_SIZE;
	ssize_t num = 0; 

	while(nleft)
	{
		if (nleft < bufsize) 
			bufsize = nleft;
		if(!bufsize)
			return -3;
		if( (num = write(fd, buf + (strLen - nleft), bufsize)) < 0)
			return -1; //Зацикливание? <=0
		nleft -= num;
	}
	return (strLen - nleft);
}

