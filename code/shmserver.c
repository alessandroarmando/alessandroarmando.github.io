//SHMServer.C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE     27

void die(const char *s) {
    perror(s);
    exit(1);
}

int main() {
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    key = 5678;

    if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
        die("shmget");

    if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)0)
        die("shmat");

    /*
     *      * Put some things into the memory for the
     *        other process to read.
     *        */
    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;


    /*
     * Wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (*shm != '*')
        sleep(1);

    exit(0);
}
