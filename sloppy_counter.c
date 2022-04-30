#include "pthread.h"

#define NUMCPUS 12

typedef struct counter_t {
  int global;                      // global count
  pthread_mutex_t glock;           // global lock
  int local[NUMCPUS];              // local count (per cpu)
  pthread_mutex_t llock[NUMCPUS];  // ... and locks
  int threshold;                   // update frequency
} counter_t;

void init(counter_t* c, int threshold) {
  c->threshold = threshold;

  c->global = 0;
  pthread_mutex_init(&c->glock, NULL);

  int i;
  for (i = 0; i < NUMCPUS; i++) {
    c->local[i] = 0;
    pthread_mutex_init(&c->llock[i], NULL);
  }
}

void update(counter_t* c, int threadID, int amt) {
  pthread_mutex_lock(&c->llock[threadID]);
  c->local[threadID] += amt;  // assumes amt > 0
  if (c->local[threadID] >= c->threshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->local[threadID];
    pthread_mutex_unlock(&c->glock);
    c->local[threadID] = 0;
  }
  pthread_mutex_unlock(&c->llock[threadID]);
}

int get(counter_t* c) {
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);
  return val;  // only approximate!
}

int main(int argc, char* argv[]) {
  return 0;
}