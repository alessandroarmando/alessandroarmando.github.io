--- 
layout: default
title: "Calcolatori Elettronici (a.y. 2017-2018) - Interprocess Communication (IPC)"
---

# Calcolatori Elettronici (a.y. 2017-2018) - Interprocess Communication (IPC)


## 1. Introduction

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


### 1.1. Shared Memory




