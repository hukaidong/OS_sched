#ifndef MY_PTHREAD_QUEUE_H
#define MY_PTHREAD_QUEUE_H

#include "utils/utils.h"
#include "types.h"
#include <stdbool.h>

inline void q_init(Quctx *queue);
inline bool is_empty(Quctx *queue);
inline void push(Quctx *queue, uctx_p uc);
inline uctx_p pop(Quctx *queue);

#endif  /* ifndef MY_PTHREAD_QUEUE_H */
