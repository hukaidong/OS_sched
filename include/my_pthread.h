#ifndef MY_PTHREAD_PTHREAD_H
#define MY_PTHREAD_PTHREAD_H

#define USE_MY_PTHREAD
#ifdef USE_MY_PTHREAD

#include "utils/utils.h"
#include "types.h"

#define pthread_attr_t void
#define pthread_t my_pthread_t

#define pthread_create my_pthread_create
int my_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

#define pthread_exit my_pthread_exit
void my_pthread_exit(void *retval);

#define pthread_join my_pthread_join
int my_pthread_join(pthread_t thread, void **retval);

#define pthread_yield my_pthread_yield
int my_pthread_yield(void);

#define pthread_mutexattr_t void
#define pthread_mutex_t my_pthread_mutex_t

#define pthread_mutex_init my_pthread_mutex_init
int my_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

#define pthread_mutex_lock my_pthread_mutex_lock
int my_pthread_mutex_lock(pthread_mutex_t *mutex);

#define pthread_mutex_unlock my_pthread_mutex_unlock
int my_pthread_mutex_unlock(pthread_mutex_t *mutex);

#define pthread_mutex_destroy my_pthread_mutex_destroy
int my_pthread_mutex_destroy(pthread_mutex_t *mutex);

#define EDEADLK 1
#define EINVAL 2
#define ESRCH 3

#endif /* ifdef USE_MY_PTHREAD */
#endif /* ifndef  MY_PTHREAD_PTHREAD_H */
