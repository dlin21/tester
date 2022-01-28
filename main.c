#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main() {
    printf("Parent is forking...\n");
    srand(time(NULL));

    int fork1 = fork(), fork2;
    if (fork1) {
        rand();
        fork2 = fork();
    }

    if (fork1 && fork2) {
        int status;
        int pid = wait(&status);

        printf("Parent: Child pid %d slept for %d seconds!\n", pid, WEXITSTATUS(status));
        printf("Parent is done!\n");
        return 0;
    } else {
        printf("Child pid: %d\n", getpid());

        int seconds = (rand() % 4) + 2;
        sleep(seconds);

        printf("Child slept for %d seconds\n", seconds);

        return seconds;
    }

    return 0;
}
