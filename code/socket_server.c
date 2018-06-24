#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sysexits.h>
#include <unistd.h>
#include <unistd.h>
#include <memory.h>
#include "aes.h"

#define BUF_SIZE 2048
#define MAX_CONN 1
#define PORT 8888

void print_hex(BYTE str[], int len) {
  int idx;
  for(idx = 0; idx < len; idx++)
    printf("%02x", str[idx]);
}

int main(int argc, char *argv[]) {
    int socket_desc, client_sock, read_size, size_sockaddr;
    struct sockaddr_in server , client;
    BYTE client_message[BUF_SIZE];

    // create socket descriptor
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        perror("Error");
        exit(EX_OSERR);
    }
    puts("Socket created");

    // prepare the sockaddr_sin structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) != 0) {
        perror("Error");
        exit(EX_OSERR);
    }
    puts("Bind done");

    /* 
     * listen() marks the socket as a passive socket, that is, 
     * as a socket that will be used to accept incoming connection
     */
    if (listen(socket_desc, MAX_CONN) != 0) {
        perror("Listen failed");
        exit(EX_OSERR);
    }

    // accept and incoming connection
    puts("Waiting for incoming connections...");
    size_sockaddr = sizeof(struct sockaddr_in);

    // accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&size_sockaddr);
    if (client_sock < 0) {
        perror("Error");
        exit(EX_OSERR);
    }
    puts("Connection accepted");

    // receive a message from client
    if((read_size = recv(client_sock, client_message, BUF_SIZE, 0)) > 0 ) {
      printf("read_size:%d\n",read_size);
      printf("Received Request:");
      print_hex(client_message,read_size);
      printf("\n");

      int randomData = open("/dev/urandom", O_RDONLY);
      if (randomData < 0) {
        perror("Error opening /dev/urandom.");
        exit(EX_OSERR);
      }
      else {
	char myRandomData[1];
	ssize_t result = read(randomData, myRandomData, sizeof myRandomData);
	if (result < 0)
	  {
	    perror("Reading /dev/urandom failed.");
	    exit(EX_OSERR);
	  }
	//Add challenge (nonce) to message
	//      client_message[2]=0x23;
	client_message[2]=myRandomData[0];
      }      

      //Send message back to client
      write(client_sock,client_message,3);

      //Receive response to challenge from client
      if((read_size = recv(client_sock, client_message, BUF_SIZE, 0)) > 0 ) {
      	printf("read_size:%d\n",read_size);
	printf("Received Response:");
	print_hex(client_message,read_size);
	printf("\n");

	BYTE plaintext[BUF_SIZE];
	WORD key_schedule[60];
	BYTE key[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
	printf("* Setting up the key:\n");
	aes_key_setup(key, key_schedule, 256);
	printf("Key          : ");
	print_hex(key, 32);
	printf("Decrypting Response:");
	aes_decrypt(client_message, plaintext, key_schedule, 256);
	printf("Decrypted Text:");
	print_hex(plaintext,256);
	printf("\n");
      }
      if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
      }
      else if(read_size == -1) {
        perror("Error");
        exit(EX_OSERR);
      }
      
    }

    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1) {
        perror("Error");
        exit(EX_OSERR);
    }
    return 0;
}
