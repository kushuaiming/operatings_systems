#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

int buffer;
int count = 0;

int loops = 10;

cond_t cond;    // +
mutex_t mutex;  // +

void put(int value) {
  assert(count == 0);
  count = 1;
  buffer = value;
}

int get() {
  assert(count == 1);
  count = 0;
  return buffer;
}

void* producer(void* arg) {
  int i;
  for (i = 0; i < loops; i++) {
    Pthread_mutex_lock(&mutex);                        // +
    if (count == 1) Pthread_cond_wait(&cond, &mutex);  // +
    put(i);
    Pthread_cond_signal(&cond);    // +
    Pthread_mutex_unlock(&mutex);  // +
  }
  return NULL;
}

void* consumer(void* arg) {
  int i = 0;
  for (i = 0; i < loops; i++) {
    Pthread_mutex_lock(&mutex);                        // +
    if (count == 0) Pthread_cond_wait(&cond, &mutex);  // +
    int tmp = get();
    Pthread_cond_signal(&cond);    // +
    Pthread_mutex_unlock(&mutex);  // +
    printf("%d\n", tmp);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t p, c1, c2;
  Pthread_create(&p, NULL, producer, NULL);
  Pthread_create(&c1, NULL, consumer, NULL);
  Pthread_create(&c2, NULL, consumer, NULL);  // +
  Pthread_join(p, NULL);
  Pthread_join(c1, NULL);
  Pthread_join(c2, NULL);  // +
  return 0;
}