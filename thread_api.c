/*for figure 27.1 27.2*/

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
typedef struct myarg_t {
  int a;
  int b;
} myarg_t;

typedef struct myret_t {
  int x;
  int y;
} myret_t;

void* mythread(void* arg) {
  myarg_t* m = (myarg_t*)arg;
  printf("%d %d\n", m->a, m->b);
  myret_t* r = malloc(sizeof(myret_t));
  r->x = 1;
  r->y = 2;
  return (void*)r;
}

int main(int argc, char* argv[]) {
  pthread_t p;
  myret_t* m;
  myarg_t args;
  args.a = 10;
  args.b = 20;
  Pthread_create(&p, NULL, mythread, &args);
  Pthread_join(p, (void**)&m);
  printf("returned %d %d\n", m->x, m->y);
  return 0;
}