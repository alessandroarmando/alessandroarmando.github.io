--- 
layout: default
title: "Calcolatori Elettronici (a.y. 2017-2018) - Hands-on Linux"
---

# Calcolatori Elettronici (a.y. 2017-2018) - Hands-on Linux


## 1. Getting access to a Linux machine

### 1.1 A Zero footprint Linux shell

You can start playing with a Linux shell straight away by using [JSLinux](https://bellard.org/jslinux/).  No installation on your machine is needed.  A Linux shell (albeit limited in functionality) is made avalable in your browser.

### 1.2 Accessing a remote Linux host via SSH

Prerequisite: SSH client installed on your machine. SSH is pre-installed in most Linux distributions or it can easily installed via the `apt` command. SSH client for Windows are available [here](http://www.putty.org/).

If you have a Linux machine, then you can establish a SSH session by issuing the the following command on a terminal:

```
> ssh -p 2220 -l bandit0 bandit.labs.overthewire.org
```
where `bandit.labs.overthewire.org` is the address of the remote host, `-p 2220` specified the port, and `-l bandit0` specifies the account on the host. (The password for bandit0 on bandit.labs.overthewire.org is ... bandit0).

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


