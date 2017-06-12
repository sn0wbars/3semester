#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include <mylib.h>

char fname[] = "dirinfo.txt";

int main()
{
    int key = ftok(fname, 0);
    if (key < 0)
        PERRET1(key);
    int semid = semget(key,1, IPC_CREAT | 0660);
    if (semid < 0)
        PERRET1(semget);

    struct sembuf cmd = {0, -1, 0};
    if (semop(semid, &cmd, 1) < 0)
         PERRET1(semop);

	umask(0);
    int fd = open(fname, O_RDWR, 0666);

    struct stat buf;
    fstat(fd, &buf);


    char *  point = (char *)mmap(NULL, buf.st_size , PROT_READ, MAP_SHARED, fd, 0);
    if(point == NULL)
        PERRET1(mmap);

    printf("%s\n", point);

    close(fd);
    return 0;

}

//Дополнительное задание

//SRV Клиент-сервер. Сервер рассылает задачи клиентам. Через очередь сообщений. Сделать менеджер, который запускает сервер и ему передаются уол-во клиентов. Клиенты все одинаковые. Выполняют разныве задачи. Клиент-серверное приложение через одну очередь. Собрать все что сделает клиент. Сообщение "Hello" клиент передает через очередь сообщений.  Для работы с каждым клиентом свой поток. Опции: TCP/IP. семафоры