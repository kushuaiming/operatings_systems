#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

#define MAX 1

int buffer[MAX];
int fill = 0;
int use = 0;

void put(int value) {
  buffer[fill] = value;
  fill = (fill + 1) % MAX;
}

int get() {
  int tmp = buffer[use];
  use = (use + 1) % MAX;
  return tmp;
}

int loops = 10;
sem_t empty, full;  // tow condition variable
sem_t mutex;

void* producer(void* arg) {
  int i;
  for (i = 0; i < loops; i++) {
    sem_wait(&empty);
    sem_wait(&mutex);
    put(i);
    sem_post(&mutex);
    sem_post(&full);
  }
  return NULL;
}

void* consumer(void* arg) {
  int i = 0;
  for (i = 0; i < loops; i++) {
    sem_wait(&full);
    sem_wait(&mutex);
    int tmp = get();
    sem_post(&mutex);
    sem_post(&empty);
    printf("%d\n", tmp);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  sem_init(&empty, 0, MAX);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

  pthread_t p, c;
  Pthread_create(&p, NULL, producer, NULL);
  Pthread_create(&c, NULL, consumer, NULL);
  Pthread_join(p, NULL);
  Pthread_join(c, NULL);
  return 0;
}