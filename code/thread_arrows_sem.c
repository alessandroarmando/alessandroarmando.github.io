#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

sem_t s;

void* print_rightarrow (void* unused) {
  while (1) {
    sem_wait(&s);
    fputc ('-', stdout);
    fputc ('>', stdout);
    fputc ('\n', stdout);
    sem_post(&s);
  }
    return NULL;
}


int main () {
    pthread_t thread_id;
    sem_init(&s,0,1);
    
    pthread_create(&thread_id, NULL, &print_rightarrow, NULL);

    while (1) {
      sem_wait(&s);
      fputc ('<', stdout);
      fputc ('=', stdout);
      fputc ('\n', stdout);
      sem_post(&s);
    }
    return 0;
}
