#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    char string[] = "Hello, world!\n";
    int fd = open("hello.txt", O_WRONLY | O_CREAT);
    write(fd, string, sizeof(string));
    close(fd);
    return 0;
}
