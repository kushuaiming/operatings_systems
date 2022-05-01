#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

#define MAX 5

int buffer[MAX];
int fill = 0;
int use = 0;
int count = 0;

void put(int value) {
  buffer[fill] = value;
  fill = (fill + 1) % MAX;
  count++;
}

int get() {
  int tmp = buffer[use];
  use = (use + 1) % MAX;
  count--;
  return tmp;
}

int loops = 10;
cond_t empty, full;  // tow condition variable
mutex_t mutex;

void* producer(void* arg) {
  int i;
  for (i = 0; i < loops; i++) {
    Pthread_mutex_lock(&mutex);
    while (count == MAX) Pthread_cond_wait(&empty, &mutex);
    put(i);
    Pthread_cond_signal(&full);
    Pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void* consumer(void* arg) {
  int i = 0;
  for (i = 0; i < loops; i++) {
    Pthread_mutex_lock(&mutex);
    while (count == 0) Pthread_cond_wait(&full, &mutex);
    int tmp = get();
    Pthread_cond_signal(&empty);
    Pthread_mutex_unlock(&mutex);
    printf("%d\n", tmp);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t p, c1, c2;
  Pthread_create(&p, NULL, producer, NULL);
  Pthread_create(&c1, NULL, consumer, NULL);
  Pthread_create(&c2, NULL, consumer, NULL);
  Pthread_join(p, NULL);
  Pthread_join(c1, NULL);
  Pthread_join(c2, NULL);
  return 0;
}