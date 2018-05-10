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
The implementation of the following [client](code/shmclient.c) and [server](code/shmserver.c) makes also use of shared memory.

## 1.2. Mapped Memory

*Mapped memory* permits different processes to communicate via a shared file.

Although you can think of mapped memory as using a shared memory segment
with a name, there are technical differences.

Mapped memory can be used for interprocess communication or as an easy way to access
the contents of a file.

Mapped memory forms an association between a file and a process's memory.

Linux splits the file into page-sized chunks and then copies them into virtual memory
pages so that they can be made available in a process's address space.

Thus, the process can read the file’s contents with ordinary memory access.
It can also modify the file's contents by writing to memory.

This permits fast access to files.

You can think of mapped memory as allocating a buffer to hold a file's entire contents, and then reading the file into the buffer and (if the buffer is modified) writing
the buffer back out to the file afterward.

Linux handles the file reading and writing operations for you.

### Mapping an Ordinary File

```
void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset);
```
where
- `addr` is the address at which you would like Linux to map the file into your process’s address space; the value NULL allows Linux
to choose an available start address.
- `length` is the length of the map in bytes.
- `prot` specifies the protection on the mapped address range (bitwise “or” of `PROT_READ`, `PROT_WRITE`, and `PROT_EXEC`).
- `flags` is a flag value that specifies additional options.
- `fd` is a file descriptor opened to the file to be mapped.
- `offset` is the offset from the beginning of the file from which to start the map.

You can map all or part of the file into memory by choosing the starting offset and length appropriately.

When you’re finished with a memory mapping, release it by using `munmap`: 
```
int munmap(void *addr, size_t length);
```

### Example:
The first program [02_mmap-write.c](code/02_mmap-write.c) generates a random number and writes it to a memory-mapped file.

The second program [02_mmap-read.c](code/02_mmap-read.c) reads the number, prints it, and replaces it in the memory-mapped file with double the value.

Both take a command-line argument of the file to map.

```
> ./02_mmap-write.exe integer-file
> cat integer-file
15
> ./02_mmap-read.exe integer-file
value:15
> cat integer-file
30
```

## 1.3. Pipes

A pipe is a communication device that permits unidirectional communication:
- Data written to the “write end” of the pipe is read back from the “read end.”
- Pipes are serial devices: the data is always read from the pipe in the same order it was written.
- Typically, a pipe is used to communicate between two threads in a single process or
between parent and child processes.

In a shell, the symbol `|` creates a pipe. For example, this shell command causes the shell to produce two child processes, one for `ls` and one for `less`:
```
> ls | less
```
The shell also creates a pipe connecting the standard output of the `ls` subprocess with
the standard input of the `less` process.

The filenames listed by `ls` are sent to less in exactly the same order as if they were sent directly to the terminal.

A pipe’s data capacity is limited:
- If the writer process writes faster than the reader
process consumes the data, and if the pipe cannot store more data, the writer process
blocks until more capacity becomes available.
- If the reader tries to read but no data is
available, it blocks until data becomes available.

Thus, the pipe automatically synchronizes the two processes.

### 1.3.1. Creating Pipes

To create a pipe, invoke the `pipe` command. Supply an integer array of size 2.
The call to `pipe` stores the reading file descriptor in array position 0 and the writing file
descriptor in position 1.

For example, consider this code:
```
int pipe_fds[2];
int read_fd, int write_fd;

pipe(pipe_fds);
read_fd = pipe_fds[0];
write_fd = pipe_fds[1];
```

Data written to the file descriptor `read_fd` can be read back from `write_fd`.

### 1.3.2. Communication Between Parent and Child Processes

A call to pipe creates file descriptors, which are valid only within that process and its
children.

A process’s file descriptors cannot be passed to unrelated processes; however,
when the process calls `fork`, file descriptors are copied to the new child process.

Thus, pipes can connect only related processes.

### Example
In the program [04_pipe.c](code/04_pipe.cx), a fork spawns a child process.

The child inherits the pipe file descriptors.

The parent writes a string to the pipe, and the child reads it out.

The sample program converts these file descriptors into `FILE*` streams using `fdopen`.
This allows for the use of higher-level standard C library I/O functions such as `printf` and `fgets`.

## 1.4. FIFOs (Named Pipes)

A first-in, first-out (FIFO) file is a pipe that has a name in the filesystem.

Any process can open or close the FIFO.

The processes on either end of the pipe need not be related to each other.

FIFOs are also called named pipes.

You can make a FIFO using the `mkfifo` command.
```
> mkfifo /tmp/fifo
> ls -la /tmp/fifo
prw-rw-r-- 1 armando armando 0 mag 10 10:28 /tmp/fifo
```

In one window, read from the FIFO by invoking the following:
```
> cat < /tmp/fifo
```

In a second window, write to the FIFO by invoking this:
```
> cat > /tmp/fifo
```

Then type in some lines of text.

Each time you press Enter, the line of text is sent through the FIFO and appears in the first window.

Close the FIFO by sending a EOF (press Ctrl+D) in the second window.

Remove the FIFO with this line:
```
> rm /tmp/fifo
```

### 1.4.1. Creating a FIFO

Create a FIFO programmatically using the `mkfifo` function:
```
int mkfifo(const char *pathname, mode_t mode);
```
where
- `pathname` is the path at which to create the FIFO;
- mode specifies the pipe’s owner, group, and world permissions. (Since a pipe must have a reader and a writer, the permissions must include both read and write permissions.)

### 1.4.2. Accessing a FIFO

Access a FIFO just like an ordinary file.

To communicate through a FIFO, one program must open it for writing, and another program must open it for reading.

For example, to write a buffer of data to a FIFO using low-level I/O routines, you
could use this code:
```
int fd = open (fifo_path, O_WRONLY);
write (fd, data, data_length);
close (fd);
```

To read a string from the FIFO using C library I/O functions, you could use
this code:
```
FILE* fifo = fopen (fifo_path, “r”);
fscanf (fifo, “%s”, buffer);
fclose (fifo);
```
A FIFO can have multiple readers or multiple writers. Bytes from each writer are
written atomically up to a maximum size of `PIPE_BUF` (4KB on Linux). Similar rules apply to simultaneous reads.

## 1.5. Sockets

A socket is a bidirectional communication device that can be used to communicate with
another process on the same machine or with a process running on other machines.

Internet programs such as Telnet, rlogin, FTP, talk, and the World Wide Web use sockets.

For example, you can obtain the WWW page from a Web server using the
Telnet program because they both use sockets for network communications. 4
To open a connection to a WWW server at `www.codesourcery.com`, use
```
> telnet www.codesourcery.com 80
Trying 107.23.79.96...
Connected to www-redirect.mentor-esd.com.
Escape character is '^]'.
GET
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
<title>301 Moved Permanently</title>
</head><body>
<h1>Moved Permanently</h1>
<p>The document has moved <a href="http://www.mentor.com/">here</a>.</p>
</body></html>
Connection closed by foreign host.
```

### 1.5.1. System Calls

These are the system calls involving sockets:
- `socket`: Creates a socket
- `close`: Destroys a socket
- `connect`: Creates a connection between two sockets
- `bind`: Labels a server socket with an address
- `listen`: Configures a socket to accept conditions
- `accept`: Accepts a connection and creates a new socket for the connection

Sockets are represented by file descriptors.

The `socket` and `close` functions create and destroy sockets, respectively:
```
int socket(int domain, int type, int protocol);
int close(int fd);
```
where
- `domain` specifies a communication domain (e.g. `AF_UNIX` and `AF_LOCAL` for local communication and `AF_INET` for Internet Protocols), i.e. the protocol family which will be used for communication.
- `type` specifies the communication semantics (e.g. `SOCK_STREAM` for sequenced, reliable, two-way, connection-based byte streams and `SOCK_DGRAM` fr datagrams, i.e connectionless, unreliable messages of a fixed maximum length).
- `protocol` specifies a particular protocol to be used with the socket.  Normally only a single protocol exists to support a particular socket type within a given protocol family, in which case protocol can be specified as 0.

To create a connection between two sockets, the client calls `connect`:
```
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
where
- `sockfd` is the the file description of the socket
- `addr` is the address of the server
- `addrlen` specifies the size of addr

The client is the process initiating the connection and the server is the process waiting to accept connections.

The client calls `connect` to initiate a connection from a local socket to the server socket whose address is `addr`.

A server’s life cycle consists of
- the creation of a connection-style socket,
- binding an address to its socket,
- placing a call to listen that enables connections to the socket,
- placing calls to accept incoming connections, and then
- closing the socket.


An address must be bound to the server’s socket using `bind`:
```
 int bind(int sockfd, const struct sockaddr *addr,
          socklen_t addrlen);
```		
where
- `sockfd` is the socket file descriptor,
- `addr` is a pointer to a socket address structure
- `addrlen` is the length of the address structure, in bytes.

When an address is bound to a connection-style socket, it must invoke `listen` to indicate that it is a server:
```
int listen(int sockfd, int backlog);
```
where
- `sockfd` is the socket file descriptor,
- `backlog` specifies how many pending connections are queued. If the queue is full, additional connections will be rejected.  (This does not limit the total number of connections that a server can handle; it limits just the number of clients attempting to connect that have not yet
been accepted.)

A server accepts a connection request from a client by invoking `accept`:
```
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
where
- `sockfd` is the socket file descriptor,
- `addr` is a pointer to a socket address structure
- `addrlen` is the length of the address structure, in bytes.

The server can use the client address to determine whether it really wants to communicate with the client.

The call to `accept` creates a new socket for communicating with the client and returns the corresponding file descriptor.

The original server socket continues to accept new client connections.

### Example
Server [07_socket_server.c](code/07_socket_server.c) and client [07_socket_client.c](code/07_socket_client.c) using sockets.
