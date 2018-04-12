#include <pthread.h>
#include <stdio.h>

void* print_rightarrow (void* unused) {
  while (1) {
    fputc ('-', stdout);
    fputc ('>', stdout);
    fputc ('\n', stdout);
  }
    return NULL;
}

int main () {
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, &print_rightarrow, NULL);

    while (1) {
      fputc ('<', stdout);
      fputc ('=', stdout);
      fputc ('\n', stdout);
    }
    return 0;
}
