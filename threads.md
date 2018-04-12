--- 
layout: default
title: "Calcolatori Elettronici (a.y. 2017-2018) - Linux Threads"
---

# Calcolatori Elettronici (a.y. 2017-2018) - Linux Threads


## 1. Introduction

As with processes the Linux kernel schedules threads asynchronously, interrupting each thread from time to time to give others a chance to execute.

When you invoke a program, Linux creates a new process and in that process creates a single thread, which runs the program sequentially.
* That thread can create additional threads;
* all these threads run the same program in the
same process,
* but each thread may be executing a different part of the program at any given time.

When a program forks a child process, the child process initially
runs its parent’s program, with its parent’s virtual memory, file descriptors, and so
on copied.

The child process can modify its memory, close file descriptors, and the like
without affecting its parent, and vice versa.

When a program creates another thread nothing is copied.
The creating and the created thread share the same memory
space, file descriptors, and other system resources as the original.

If one thread changes the value of a variable, for instance, the other thread subsequently will see the modified value.

Similarly, if one thread closes a file descriptor, other threads may not read from or write to that file descriptor.

Because a process and all its threads can be executing only one program at a time, if any thread inside a process calls one of the `exec` functions, all the other existing threads are ended.

## 2 Thread Creation

Each thread in a process is identified by a thread ID. When referring to thread IDs in
C or C++ programs, use the type `pthread_t`.

### 2.1 Thread Functions

Upon creation, each thread executes a *thread function*:
* This is an ordinary function that contains the code that the thread should run.
* Thread functions take a single parameter, of type `void *` and have a `void *` return type.  For example
```
void* print_xs (void* c) {
  char *pch=(char *)c; // type casting
  while (1)
    fputc (*pch, stdout);
  return NULL;
}
```
* The parameter is the thread argument: the OS passes the value from the calling thread to the called thread without looking at it.
* When the thread function returns, the thread exits.

The `pthread_create` function creates a new thread. The function requires as input:

1. A pointer to a pthread_t variable, in which the thread ID of the new thread is
stored.
2. A pointer to a thread attribute object.This object controls details of how the
thread interacts with the rest of the program. If you pass NULL as the thread
attribute, a thread will be created with the default thread attributes.
3. A pointer to the thread function. 
4. A thread argument value of type `void *`. Whatever you pass is simply passed as
the argument to the thread function when the thread begins executing.

A call to `pthread_create` returns immediately, and the original thread continues executing the instructions following the call.

Meanwhile, the new thread begins executing the thread function.

Linux schedules both threads asynchronously, and your program
must not rely on the relative order in which instructions are executed in the two
threads.


### 1.3 Installing a Linux distribution on your machine

You can create a local, disposable copy of Linux by using the virtualization technology.  Many virtualization solutions are available today, the most prominent being [VMware](https://www.vmware.com) and [VirtualBox](https://www.virtualbox.org/).

## 2. Basic Linux shell commands

Learn to use and play with the following commands:

* pwd - Print name of current working directory
* cd - Change directory
* ls - List directory contents
* file - Determine file type
* less - View file contents
* cp - Copy files and directories
* mv - Move/rename files and directories
* mkdir - Create directories
* rm - Remove files and directories
* which - Display which executable program will be executed
* man - Display a command's manual page
* whatis - Display a very brief description of a command
* cat - Concatenate files
* sort - Sort lines of text
* uniq - Report or omit repeated lines
* grep - Print lines matching a pattern
* find - Find...
* wc - Print newline, word, and byte counts for each file
* head - Output the first part of a file
* tail - Output the last part of a file
* tee - Read from standard input and write to standard output and files

## 3. Play with Bandit

Improve your hands-on skills by taking on the [Bandit](http://overthewire.org/wargames/bandit/) challenges.

## 4. Advanced Linux shell commands

 * Standard Input, Output, And Error
 * I/O Redirection
   - Redirecting Standard Output

     * ``ls -l /usr/bin > ls-output.txt``
     * ```
     > ls-output.txt
     ls -l /usr/bin >> ls-output.txt
     ls -l /usr/bin >> ls-output.txt
     ls -l /usr/bin >> ls-output.txt
```
   - Redirecting Standard Error
     * ``ls -l /bin/usr > ls-output.txt``
     * ```ls -l /bin/usr 2> ls-error.txt```
   - Redirecting Standard Output And Standard Error To One File
     * ``ls -l /bin/usr > ls-output.txt 2>&1``
     * ``ls -l /bin/usr &> ls-output.txt``
     * ``ls -l /bin/usr &>> ls-output.txt``
   - Disposing Of Unwanted Output
     * ``ls -l /bin/usr 2> /dev/null``
   - Redirecting Standard Input
     * ```
       wget http://csec.it/events/boeing-ctf/photos/boeing1.jpg
       ls -la boeing1.jpg
       split --bytes=10000 boeing1.jpg
       ls -la x*
       diff boeing1.jpg xaa
       cat x?? > new-boeing1.jpg
       diff boeing1.jpg new-boeing1.jpg
       ```
     * ```
       cat ls-output.txt
       cat
       cat > readme.txt
       cat < readme.txt
       ```
 * Pipelines
   - Filters
     * ``ls /bin /usr/bin | sort | less``
   - Read From Stdin And Output To Stdout And Files
     * ``ls /usr/bin | tee ls.txt | grep zip``

