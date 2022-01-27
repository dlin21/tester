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

int main() {
    // Access shared memory and semaphore
    int semd = semget(SEMKEY,1,0);
    int shmd = shmget(SHMKEY,1,0);

    // Down the semaphore
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1; //setting the operation to down
    semop(semd, &sb, 1); //perform the operation

    // Attach shared memory data
    int *len = shmat(shmd,0,0);

    // Reposition file descriptor to read in last line
    int f = open("phone.txt",O_RDONLY);
    lseek(f,-1 * *len,SEEK_END);
    char line[4096];
    int err = read(f,line,*len);
    if(err == -1) {
        printf("Error:\n");
        printf("%s\n", strerror(errno));
        return 0;
    }
    line[*len]='\0';
    printf("Last line: %s\n",line);

    // Take in line as STDIN set len to last line size
    printf("New line: ");
    fgets(line,4096,stdin);
    *len = strlen(line);

    // Write line to phone.txt
    f = open("phone.txt", O_WRONLY | O_APPEND);
    if(f == -1) {
        printf("Error:\n");
        printf("%s\n", strerror(errno));
        return 0;
    }
    err = write(f,line,strlen(line));
    if(err == -1) {
        printf("Error:\n");
        printf("%s\n", strerror(errno));
        return 0;
    }

    // Detach segment from variable
    shmdt(len);

    // Up the semaphore
    sb.sem_op = 1; //set the operation to up
    semop(semd, &sb, 1); //perform the operation

    return 0;
}
