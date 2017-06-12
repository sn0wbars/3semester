#ifndef _LIB_H_
#define _LIB_H_

enum message_types
{
	BYE = 'B',
	DOWORK = 'D',
	WORKDONE = 'W'
};

int port = 4444;

const size_t MAX_RCV = 4096;

struct message
{
	int type;
	int* data;
};

//char defpathlog[] = "log.txt";

#endif