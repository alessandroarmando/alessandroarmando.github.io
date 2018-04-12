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

### 1.1. Posix Standard Thread API (pthreads)

Linux implements the POSIX standard thread API (pthreads).

All thread functions and data types are declared in the header file `<pthread.h>`.

The pthread functions are not included in the standard C library. Instead, they are in
`libpthread`, so you should add `-lpthread` to the command line when you link your
program.


## 2. Dealing with Threads

Each thread in a process is identified by a thread ID. When referring to thread IDs in
C or C++ programs, use the type `pthread_t`.

### 2.1 Thread Function

Upon creation, each thread executes a *thread function*:
* This is an ordinary function that contains the code that the thread should run.
* Thread functions take a single parameter, of type `void *` and have a `void *` return type.  For example
```
void* print_c (void* c) {
  char *pch=(char *)c; // type casting
  while (1)
    fputc (*pch, stdout);
  return NULL;
}
```
* When the thread function returns, the thread exits.

### 2.2 Thread Creation

The `pthread_create` function creates a new thread.
```
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
		   void *(*thread_function) (void *),
		   void *arg);
```
where the arguments are:
1. A pointer to the thread ID of the new thread.
2. A pointer to the thread function. 
3. A pointer to a thread attribute object. This object controls details of how the thread interacts with the rest of the program. (NULL to set default thread attributes.
4. A thread argument value of type `void *`. 

The call to `pthread_create` returns immediately, and the original thread continues executing the instructions following the call.

Meanwhile, the new thread begins executing the thread function.

Linux schedules both threads asynchronously, and your program MUST NOT rely on the relative order in which instructions are executed in the two threads.

```
int main () {
    pthread_t thread_id;
    char ch='x';
    char *pchar=&ch;

    pthread_create(&thread_id, NULL, &print_xs, pchar);

    while (1)
        fputc ('.', stdout);
    return 0;
}
```

### Example
The complete program is available [here](code/thread_create.c).  It creates a thread that prints x’s continuously to standard error while the main thread prints o’s continuously to standard error.

The program can be compiled and link using the following command:
```
% gcc -o thread-create thread-create.c -lpthread
```

### 2.3 Joining Threads

The program has an important bug though.  Can you spot it?

*Key Question:* What happens is the main terminates before the newly created thread?

One solution is to force main to wait until the thread is done.

What we need is a function that waits for a thread to finish.

`pthread_join` takes two arguments:
* the thread ID of the thread to wait for
* a pointer to a `void *` variable that will receive the finished thread’s return value.
If you don’t care about the thread return value, pass NULL as the second argument.

### Example (correct)
The correct version of the previous program is available [here](code/thread_create_ok.c).  

## 3. Synchronization and Critical Sections

Programming with threads is very tricky because most threaded programs are concurrent programs. 

There’s no way to know when the system will schedule one thread to run and when it will run another. One thread might run for a very long time, or the system might switch among threads very quickly.

On a system with multiple processors, the system might even schedule multiple threads to run the same time.

Debugging a threaded program is difficult because you cannot always easily reproduce the behavior that caused the problem.

You might run the program once and have everything work fine; the next time you run it, it might crash. There’s no way to make the system schedule the threads exactly the same way it did before.

The ultimate cause of most bugs involving threads is that the threads are accessing
the same resource (data, device, ...).

Tis iss one of the powerful aspects of threads, but it is also dangerous.

If one thread is only partway through updating a data structure when another thread accesses the same data structure, chaos is likely to ensue.

### Example: Interleaved Arrows
The following [program](code/thread_arrows.c) should print an alternate sequences of arrows, but it is obviously flawed. Why?

We need a way to grant each exclusive access to stdout.

How can this be achieved?

### 3.1 Mutexes

A *mutex* (MUTual EXclusion locks) is a special lock that only one thread may lock at a time.

If a thread locks a mutex and then a second thread also tries to lock the same mutex, the second thread is blocked, or put on hold.

Only when the first thread unlocks the mutex is the second thread unblocked, i.e. allowed to resume execution.

To create a mutex, create a variable of type `pthread_mutex_t` and pass a pointer to
it to `pthread_mutex_init`.
```
int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
```
The second argument is a pointer to a mutex attribute object, which specifies attributes of the mutex. (If NULL, default attributes are assumed.)

For instance,
```
pthread_mutex_t mutex;
pthread_mutex_init (&mutex, NULL);
```

Another simpler way to create a mutex with default attributes is to initialize it
with the special value `PTHREAD_MUTEX_INITIALIZER`:
```
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

A thread may attempt to lock a mutex by calling `pthread_mutex_lock` on it:
* If the mutex was unlocked, it becomes locked and the function returns immediately.
* If the mutex was locked by another thread, `pthread_mutex_lock` blocks execution and
returns only eventually when the mutex is unlocked by the other thread.

More than one thread may be blocked on a locked mutex at one time.

When the mutex is unlocked, only one of the blocked threads (chosen unpredictably) is unblocked and allowed to lock the mutex; the other threads stay blocked.

A call to `pthread_mutex_unlock` unlocks a mutex.
This function should always be called from the same thread that locked the mutex.


### Example: Interleaved Arrows (correct)
The correct version of the previous program is available [here](code/thread_create_ok.c).  

