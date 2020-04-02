#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 1000

int main() {

    int child_status;
    pid_t child_pid;
    int i;

    printf("The main program has the PID %d.\n\n", getpid());
    
    child_pid = fork();
 
    if(child_pid != 0) {
        printf("This is the parent process with PID %d.\n", getpid());
        printf("The child process has the PID %d.\n", child_pid);
	for(i=0;i<N;i++) {
	  fputc ('-', stdout);
	  fputc ('>', stdout);
	  fputc ('\n', stdout);
	}
    }
    else {
        printf("This is the child process with PID %d.\n", getpid());
        printf("Its parent process is with PID %d.\n", getppid());
	for(i=0;i<N;i++) {
	  fputc ('<', stdout);
	  fputc ('=', stdout);
	  fputc ('\n', stdout);
	}
    }

	return 0;
}
