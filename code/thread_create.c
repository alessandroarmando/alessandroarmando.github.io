#include <pthread.h>
#include <stdio.h>

void* print_xs (void* c) {
  char *pch=(char *)c; // type casting
  while (1)
    fputc (*pch, stdout);
  return NULL;
}

int main () {
    pthread_t thread_id;
    char ch='x';
    char *pchar=&ch;

    pthread_create(&thread_id, NULL, &print_xs, pchar);

    while (1)
        fputc ('.', stdout);
    return 0;
}
