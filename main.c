#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

#define SEMKEY 24602
#define SHMKEY 24603

/* union semun { */
/*   int              val;    /\* Value for SETVAL *\/ */
/*   struct semid_ds *buf;    /\* Buffer for IPC_STAT, IPC_SET *\/ */
/*   unsigned short  *array;  /\* Array for GETALL, SETALL *\/ */
/*   struct seminfo  *__buf;  /\* Buffer for IPC_INFO */
/*                               (Linux-specific) *\/ */
/* }; */

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Invalid Command\n");
    } else {
        if (!strcmp(argv[1],"-create")) {
            // Create shared memory + semaphore
            int shmd = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0644);
            int semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
            if (semd == -1) {
                printf("error %d: %s\n", errno, strerror(errno));
            }

            // Set value of semaphore to 1
            union semun us;
            us.val = 1;
            int r = semctl(semd, 0, SETVAL, us);
            if (r == -1) {
                printf("error %d: %s\n", errno, strerror(errno));
            }

            // Open file
            int file = open("phone.txt", O_CREAT | O_TRUNC, 0644);
            if(file == -1) {
                printf("Error:\n");
                printf("%s\n", strerror(errno));
                return 0;
            }
        }

        else if (!strcmp(argv[1],"-remove")) {
            // access and remove shared memory + semaphore
            int semd = semget(SEMKEY,1,0);
            int shmd = shmget(SHMKEY,1,0);
            shmctl(shmd,IPC_RMID,0);
            semctl(semd,IPC_RMID,0);

            // print contents of file
            int file = open("phone.txt",O_RDONLY);
            if(file == -1) {
                printf("Error:\n");
                printf("%s\n", strerror(errno));
                return 0;
            }
            struct stat st;
            stat("phone.txt", &st);
            char *buff = malloc(st.st_size+1);
            buff[st.st_size]='\0';

            int err = read(file,buff,st.st_size);
            if(err == -1) {
                printf("Error:\n");
                printf("%s\n", strerror(errno));
                return 0;
            }

            printf("Contents of Story:\n");
            printf("%s\n",buff);
            free(buff);
        }

        else {
            printf("invalid command, run ./control -remove or ./control -create\n");
        }
    }
    return 0;
}
