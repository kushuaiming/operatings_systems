#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

int buffer;
int count = 0;

int loops = 10;

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
    put(i);
  }
  return NULL;
}

void* consumer(void* arg) {
  int i = 0;
  while (i < loops) {
    int tmp = get();
    printf("%d\n", tmp);
    i++;
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t p, c;
  Pthread_create(&p, NULL, producer, NULL);
  Pthread_join(p, NULL);
  Pthread_create(&c, NULL, consumer, NULL);
  Pthread_join(c, NULL);
  return 0;
}