--- 
layout: default
title: "Calcolatori Elettronici (a.y. 2017-2018) - Interprocess Communication (IPC)"
---

# Calcolatori Elettronici (a.y. 2017-2018) - Interprocess Communication (IPC)


# 1. Introduction

Interprocess communication (IPC) is the transfer of data among processes.

We considere five types of interprocess communication:
* *Shared memory* permits processes to communicate by simply reading and
writing to a specified memory location.
* *Mapped memory* is similar to shared memory, except that it is associated with a
file in the filesystem.
* *Pipes* permit sequential communication from one process to a related process.
* *FIFOs* are similar to pipes, except that unrelated processes can communicate
because the pipe is given a name in the filesystem.
* *Sockets* support communication between unrelated processes even on different
computers.

These types of IPC differ by the following criteria:
* Whether they restrict communication to related processes (processes with a
common ancestor), to unrelated processes sharing the same filesystem, or to any
computer connected to a network
* Whether a communicating process is limited to only write data or only
read data
* The number of processes permitted to communicate
* Whether the communicating processes are synchronized by the IPC—for
example, a reading process halts until data is available to read

## 1.1. Shared Memory

Shared memory allows two or more processes to access the same memory as if they all
called malloc and were returned pointers to the same actual memory.

Key facts:
* When one process changes the memory, all the other processes see the
modification.
* Shared memory is the fastest form of interprocess communication because all
processes share the same piece of memory.
* Access to this shared memory is as fast as accessing a process’s nonshared memory, and it does not require a system call or entry
to the kernel.
* It also avoids copying data unnecessarily.
* Kernel does not synchronize accesses to shared memory: you must provide your own synchronization (via, e.g., semaphores).

### 1.1.1. The Memory Model

To use a shared memory segment, one process must allocate the segment. Then each
process desiring to access the segment must attach the segment. After finishing its use
of the segment, each process detaches the segment. At some point, one process must
deallocate the segment.

Under Linux, each process’s virtual memory is split into pages. Each
process maintains a mapping from its memory addresses to these virtual memory pages,
which contain the actual data. Even though each process has its own addresses, multiple
processes' mappings can point to the same page, permitting sharing of memory.

Allocating a new shared memory segment causes virtual memory pages to be created.
Because all processes desire to access the same shared segment, only one process
should allocate a new shared segment.

Allocating an existing segment does not create
new pages, but it does return an identifier for the existing pages.

To permit a process to use the shared memory segment, a process attaches it, which adds entries mapping
from its virtual memory to the segment’s shared pages.

When finished with the segment, these mapping entries are removed.

When no more processes want to access these shared memory segments, exactly one process must deallocate the virtual
memory pages.

All shared memory segments are allocated as integral multiples of the system’s page
size, which is the number of bytes in a page of memory. On Linux systems, the page
size is 4KB, but you should obtain this value by calling the `getpagesize` function.

### 1.1.2. Allocation

A process allocates a shared memory segment using `shmget` (“SHared Memory
GET”):
```
int shmget(key_t key, size_t size, int shmflg);
```
where
* `key` is an integer key that specifies which segment to create.
Unrelated processes can access the same shared segment by specifying the same key
value. If other processes choose the same fixed key, then a conflict arises. Using the special constant IPC_PRIVATE as the key value guaran-
tees that a brand new memory segment is created.
* `size` specifies the number of bytes in the segment. Because segments are allocated using pages, the number of actually allocated bytes is rounded up
to an integral multiple of the page size.
* `shmflg` specifies options (usually expressed as bitwise OR of predefined flag values).

For example, the following invocation of `shmget` creates a new shared memory segment (or
access to an existing one, if `shm_key` is already used) that’s readable and writeable to
the owner but not other users.
```
int segment_id = shmget(shm_key, getpagesize(), IPC_CREAT|S_IRUSR|S_IWUSER);
```

If the call succeeds, `shmget` returns a segment identifier. If the shared memory segment
already exists, the access permissions are verified and a check is made to ensure that
the segment is not marked for destruction.

### 1.1.3 Attachment and Detachment

To make the shared memory segment available, a process must use `shmat`, “SHared
Memory ATtach”:
```
void *shmat(int shmid, const void *shmaddr, int shmflg);
```
where
* `shmid` is the shared memory segment identifier returned by
`shmget`.
* `shmaddr` is a pointer that specifies where in your process’s address
space you want to map the shared memory; if you specify NULL, Linux will choose
an available address.
* `shmflg` specifies options

If the call succeeds, it returns the address of the attached shared segment.

When you’re finished with a shared memory segment, the segment should be
detached using `shmdt` (“SHared Memory DeTach”):
```
int shmdt(const void *shmaddr);
```
where
* `shmaddr` is the address returned by `shmat`.

If the segment has been detached and this was the last process using it, it is
removed.

### Example: Simple
The following [program](code/shm.c) makes use of shared memory.

### Example: Client-Server
The implementation of the following [client](code/shmclient.c) and [server](shmserver.c) makes also use of shared memory.
