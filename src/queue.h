#ifndef MY_PTHREAD_QUEUE_H
#define MY_PTHREAD_QUEUE_H

#include <stdbool.h>
#include "type.h"

#define MAX_Q_SIZE 20

typedef ucontext_t* uctx_p;

typedef struct {
  uctx_p  uctx[MAX_Q_SIZE];
  int head;
  int size;
} Quctx;

inline void q_init(Quctx *queue) {
  LOG(q_init);
  queue->head = 0;
  queue->size = 0;
}

inline bool is_empty(Quctx *queue) {
  //LOG(is_empty);
  return queue->size == 0;
}

inline void push(Quctx *queue, uctx_p uc) {
  LOG(push);
  int idx = (queue->head + queue->size) % MAX_Q_SIZE;
  queue->uctx[idx] = uc;
  queue->size++;
}

inline uctx_p pop(Quctx *queue) {
  LOG(pop);
  uctx_p temp = queue->uctx[queue->head];
  queue->head = (queue->head + 1) % MAX_Q_SIZE;
  queue->size--;
  return temp;
}

#endif  /* ifndef MY_PTHREAD_QUEUE_H */
