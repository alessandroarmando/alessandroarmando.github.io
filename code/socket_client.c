#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sysexits.h>
#include <unistd.h>
#include <memory.h>
#include "aes.h"
 
#define PORT 8888
#define BUF_SIZE 2048

void print_hex(BYTE str[], int len) {
  int idx;
  for(idx = 0; idx < len; idx++)
    printf("%02x", str[idx]);
}

int main(int argc , char *argv[]) {
    int sock;
    struct sockaddr_in server;
    BYTE message[BUF_SIZE], server_reply[BUF_SIZE];
     
    // create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
		perror("Error");
        exit(EX_OSERR);
    }
    puts("Socket created");
    
    // 127.0.0.1 -> https://en.wikipedia.org/wiki/Localhost
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
 
    // connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Error");
        exit(EX_OSERR);
    }
     
    puts("Connected\n");
     
    // keep communicating with server
    // reset buffers
    memset(message, '\0', sizeof(message));
    memset(server_reply, '\0', sizeof(server_reply));

    const BYTE client_id=0x01;
    const BYTE server_id=0xF0;
    
    /* printf("Sending Request-1:%02x %02x\n",client_id,server_id); */
    message[0]=client_id;
    message[1]=server_id;
    printf("Sending Request-1:");
    print_hex(message,2);
    printf("\n");
    
    if( send(sock, message, 2, 0) == -1) {
      perror("Error sending 1st message.");
      exit(EX_OSERR);
    }
    
    // receive a reply from the server
    if( recv(sock, server_reply, BUF_SIZE, 0) == -1) {
      perror("Error");
      exit(EX_OSERR);
    }
    printf("Server Reply:");
    print_hex(server_reply,3);
    printf("\n");

    WORD key_schedule[60], idx;
    BYTE enc_buf[128];
    /* BYTE plaintext[16]; */
    BYTE key[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
    printf("* Setting up the key:\n");
    aes_key_setup(key, key_schedule, 256);
    printf(  "Key          : ");
    print_hex(key, 32);
    
    aes_encrypt(server_reply, enc_buf, key_schedule, 256);
    printf("\nPlaintext    : ");
    print_hex(server_reply, 16);
    printf("\n-encrypted to: ");
    print_hex(enc_buf, 16);
    printf("\n");

    if( send(sock, enc_buf, 16, 0) == -1) {
      perror("Error sending response to challenge.");
      exit(EX_OSERR);
    }

    printf("Response to challenge sent.\n");
    
    close(sock);
    return 0;
}
