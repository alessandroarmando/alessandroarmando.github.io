#include <pthread.h>
#include <stdio.h>

#define N 1000

void* print_xs (void* c) {
  char *pch=(char *)c; // type casting
  int i;
  for(i=0;i<N;i++)
    fputc (*pch, stdout);
  return NULL;
}

int main () {
  int i;
  pthread_t thread_id;
  char ch='x';
  char *pchar=&ch;
  
  pthread_create(&thread_id, NULL, &print_xs, pchar);
  
  for(i=0;i<N;i++)
    fputc ('.', stdout);
  return 0;
}
