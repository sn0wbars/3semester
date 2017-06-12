#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <bufrw.h>
#include <mylib.h>

char fname[] = "dirinfo.txt";

struct finfo
{
    char * type;
    char * mask;
    char * owner;
    char * growner;
    char * crdate;
    char * moddate;
    char * size;
    char * name;
};

char* defaultPath = "/";

int main(int argc, char ** argv)
{

    struct finfo * fin = NULL;

    char* dirpath = (argc == 1)? defaultPath : argv[1]; 
    int fdrw[2];

    if(pipe(fdrw) == -1)
        PERRET1(pipe);


    DIR * dir = opendir(argv[1]);
    if(dir == NULL)
        PERRET1(opendir);

    umask(0);
    int fd = open(fname, O_CREAT | O_TRUNC | O_RDWR, 0666);

    struct dirent ** arr = NULL;
    int n = 0;
    do
    {
        arr = realloc(arr, (n + 1) * sizeof(struct dirent *));
        arr[n] = readdir(dir);
        if (arr == NULL)
            PERRET1(realloc);
    }
    while(arr[n++]);
  
    --n;

    if(errno)
        PERRET1(readdir);


    fin = malloc(sizeof(struct finfo) * n);
    if(fin == NULL)
        PERRET1(malloc);

    int parent = fork();
    if(parent == -1)
        PERRET1(fork);

    else if(!parent)
    {
        close(fdrw[0]);
        int len = 0;

        for(int i = 0; arr[i]; i++)
            len += strlen(arr[i]->d_name) + strlen(argv[1]) + strlen(" ");

        char* cmd = "stat -c \"%F\n%A\n%U\n%G\n%w\n%y\n%s\n%n\" ";

        char * command = calloc(strlen(cmd) + len + 1, sizeof(char));
        strcpy(command, cmd);
  
        for(int i = 0; i < n; i++)
        {
            strcat(command, argv[1]);
            strcat(command, arr[i]->d_name);
            strcat(command, " ");
        }

        dup2(fdrw[1], 1);

        if(execlp(getenv("SHELL"), getenv("SHELL"), "-c", command, NULL) == -1)
            PERRET1(execlp);

        return 0;
    }
    else
    {
        close(fdrw[1]);

        char * p = NULL;
        char * str = NULL;
        bufread(fdrw[0], &str);
        //printf("%s\n", str);
        close(fdrw[0]);

        int m = 0, size = 0, sizeall = 0;
        for(int j = 0; j < n; j++)
            for(int i = 0, size = 0; i < 8; i++)
            {
                while(str[m] != '\n' && str[m] != '\0')
                {
                    size++;
                    p = realloc(p, sizeof(char) * size);
                    p[size - 1] = str[m];
                    m++;
                }

                size++;
                p = realloc(p, sizeof(char) * size);
                p[size - 1] = '\0';
                m++;

                switch(i)
                {
                    case 0:
                        fin[j].type = p;
                    break;
                    case 1:
                        fin[j].mask = p;
                    break;
                    case 2:
                        fin[j].owner = p;
                    break;
                    case 3:
                        fin[j].growner = p;
                    break;
                    case 4:
                        fin[j].crdate = p;
                    break;
                    case 5:
                        fin[j].moddate = p;
                    break;
                    case 6:
                        fin[j].size = p;
                    break;
                    case 7:
                        fin[j].name = p;
                    break;
                }
                sizeall += size;
                p = NULL;
            }

         for(int i = 0; i < sizeall; i++)
            if(write(fd, "", 1) == -1)
                PERRET1(write);

        char *  point = (char *)mmap(NULL, sizeall, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if(point == NULL)
            PERRET1(mmap);

        strcpy(point, str);

        if(munmap(point, m) == -1)
          PERRET1(munmap);

        close(fd);

        int key = ftok(fname, 0);
        if (key < 0)
            PERRET1(key);
        int semid = semget(key,1, IPC_CREAT | 0660);
        if (semid < 0)
            PERRET1(semget);

        struct sembuf cmd = {0, 1, 0};
        if (semop(semid, &cmd, 1) < 0)
            PERRET1(semop);
            
        return 0;
    }

    return 0;
}
                                                                                            