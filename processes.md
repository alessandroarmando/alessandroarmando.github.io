--- 
layout: default
title: "Calcolatori Elettronici (a.y. 18-19) - Linux Processes"
---

# Calcolatori Elettronici (a.y. 18-19) - Linux Processes


## 1. Introduction

Each process in a Linux system is identified by its unique process ID, sometimes
referred to as pid.

Process IDs are 16-bit numbers that are assigned sequentially by Linux as new processes are created.

Every process also has a parent process (except the special init process).
Thus, you can think of the processes on a Linux system as arranged in a tree, with the init process at its root.

The parent process ID, or ppid, is simply the process ID of the process’s parent.

When referring to process IDs in a C or C++ program, always use the ``pid_t``
typedef, which is defined in ``<sys/types.h>``.

A program can obtain the process ID of the process it’s running in with the ``getpid()`` system call, and it can obtain the process
ID of its parent process with the ``getppid()`` system call.

```
#include <stdio.h>
#include <unistd.h>
int main () {
printf("The process ID is %d\n", (int) getpid ());
printf("The parent process ID is %d\n", (int) getppid ());
return 0;
}
```

## Using ``fork`` and ``exec``

Linux provides one function, ``fork``, that makes a child process that is an exact copy of its parent process.

Linux provides another set of functions, the ``exec`` family, that causes a particular process to cease being an instance of one program and to instead become an instance of another program.

To spawn a new process, you first use ``fork`` to make a copy of the current process, then you use ``exec`` to transform one of these processes into an instance of the program you want to spawn.


When a program calls ``fork``, a duplicate process, called the child process, is created:

* The parent process continues executing the program from the point that fork was called.
* The child process, too, executes the same program from the same place.

So how do the two processes differ? First, the child process is a new process and
therefore has a new process ID, distinct from its parent’s process ID.

One way for a program to distinguish whether it’s in the parent process or the child process is to call ``getpid``. However, the ``fork`` function provides different return values to the parent and child processes:

* one process “goes in” to the fork call, and
* two processes “come out,” with different return values.

Important:
* The return value in the parent process is the process ID of the child.
* The return value in the child process is zero.

Because no process ever has a process ID of zero, this makes it easy for the program whether it is now running as the parent or the child process.

```
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    int child_status;
    pid_t child_pid;

    printf("The main program has the PID %d.\n\n", getpid());
    
    child_pid = fork();
    /*
     * fork() creates a new process. 
     * The return value is 0 in the child and the pid of the child in the parent
     */
 
    if(child_pid != 0) {
        printf("This is the parent process with PID %d.\n", getpid());
        printf("The child process has the PID %d.\n", child_pid);
        wait(&child_status); // waits for the first child to die (any one!)
    }
    else {
        printf("This is the child process with PID %d.\n", getpid());
        printf("Its parent process is with PID %d.\n", getppid());
    }

	return 0;
}

```

---
### Exercise

Solve the following [exercise](code/ce-so-fork-01-nosolution.pdf).

Here is the [solution](code/ce-so-fork-01-solution.pdf).
---

### Using the ``exec`` family

The ``exec`` functions replace the program running in a process with another program.

When a program calls an ``exec`` function, that process immediately ceases executing that
program and begins executing a new program from the beginning, assuming that the
exec call doesn’t encounter an error.

### Using ``fork`` and ``exec`` together

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>


int spawn(char* program_name, char** arg_list)
{
    pid_t child_pid;

    child_pid = fork();

    if(child_pid != 0) {
        return child_pid; // if I am the parent, return the child pid
    }
    else {
        execvp(program_name, arg_list);
        /* The execvp will never return unless error occurs */
        fprintf(stderr, "An error occured when invoking execvp.\n");
        exit(EX_OSERR);
    }
}


int main()
{
    int child_status, child_pid;

    char* arg_list[] = {
        "ls",
        "-l",
        "/",
        NULL
    };

    child_pid = spawn(arg_list[0], arg_list);
    
    waitpid(child_pid, &child_status, 0); // wait for the child whose PID is equal to child_pid

    /*
     * if you have only one child you can use:
     * wait (&child_status);
     */

    if(WIFEXITED(child_status)) {
        printf("The child process exited normally with exit code %d.\n", WEXITSTATUS(child_status));
    }
    else {
        printf("The child process exited abnormally.\n");
    }

    printf("Done with the main program.\n");

    return 0;
}
```