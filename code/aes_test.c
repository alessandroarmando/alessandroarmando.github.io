/*********************************************************************
* Filename:   aes_test.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding AES
              implementation. These tests do not encompass the full
              range of available test vectors and are not sufficient
              for FIPS-140 certification. However, if the tests pass
              it is very, very likely that the code is correct and was
              compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <memory.h>
#include "aes.h"

/*********************** FUNCTION DEFINITIONS ***********************/
void print_hex(BYTE str[], int len)
{
	int idx;

	for(idx = 0; idx < len; idx++)
		printf("%02x", str[idx]);
}

int main(int argc, char *argv[]) {
  WORD key_schedule[60], idx;
  BYTE enc_buf[128];
  BYTE plaintext[16] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

  BYTE ciphertext[16] = {0xf3,0xee,0xd1,0xbd,0xb5,0xd2,0xa0,0x3c,0x06,0x4b,0x5a,0x7e,0x3d,0xb1,0x81,0xf8};
  BYTE key[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
  int pass = 1;
  
  printf("* Setting up the key:\n");
  aes_key_setup(key, key_schedule, 256);
  printf(  "Key          : ");
  print_hex(key, 32);
  
  aes_encrypt(plaintext, enc_buf, key_schedule, 256);
  printf("\nPlaintext    : ");
  print_hex(plaintext, 16);
  printf("\n-encrypted to: ");
  print_hex(enc_buf, 16);
  pass = pass && !memcmp(enc_buf, ciphertext, 16);
  
  aes_decrypt(ciphertext, enc_buf, key_schedule, 256);
  printf("\nCiphertext   : ");
  print_hex(ciphertext, 16);
  printf("\n-decrypted to: ");
  print_hex(enc_buf, 16);
  pass = pass && !memcmp(enc_buf, plaintext, 16);
  
  printf("\n\n");

return(0);
}


