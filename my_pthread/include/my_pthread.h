#ifndef MY_PTHREAD_PTHREAD_H
#define MY_PTHREAD_PTHREAD_H

#ifdef USE_MY_PTHREAD
#define _XOPEN_SOURCE  // OSX compatibility


#include "type.h"

#define pthread_attr_t void
#define pthread_t fib_p

#define pthread_create my_pthread_create
// int pthread_create(
  // pthread_t *thread,
  // const pthread_attr_t *attr,
  // void *(*start_routine) (void *),
  // void *arg);
int my_pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine) (void *),
    void *arg);

#define pthread_exit my_pthread_exit
// void pthread_exit(void *retval);
void my_pthread_exit(void *retval);

#define pthread_join my_pthread_join
// int pthread_join(pthread_t thread, void **retval);
int my_pthread_join(pthread_t thread, void **retval);

#define pthread_yield my_pthread_yield
// int pthread_yield(void);
int my_pthread_yield(void);

#define EDEADLK 1
#define EINVAL 2
#define ESRCH 3

#endif /* ifdef USE_MY_PTHREAD */

#endif /* ifndef  MY_PTHREAD_PTHREAD_H */
