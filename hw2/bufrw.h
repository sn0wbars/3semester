#ifndef _BUFRW_
#define _BUFRW_

ssize_t bufread(int fd, char ** p_buf);
ssize_t bufwrite(int fd, char* buf);

#endif