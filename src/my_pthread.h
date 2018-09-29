#ifndef MY_PTHREAD_H
#define MY_PTHREAD_H

#ifdef USE_MY_PTHREAD
#define _XOPEN_SOURCE // OSX compatibility

#include <unistd.h>
#include <ucontext.h>

#define pthread_t my_pthread_t
#define pthread_ptr my_pthread_ptr
typedef struct thread {
  ucontext_t *uc_link;
  struct thread *next;
} my_pthread_t, *my_pthread_ptr;


// #undef pthread_attr_t
// typedef void *pthread_attr_t;  // ALWAYS NULL

#define pthread_mutex_t my_pthread_mutex_t
#define pthread_mutex_ptr my_pthread_mutex_ptr
typedef struct thread_mutex {
  ucontext_t *owner;
  struct thread_mutex *next;
} my_pthread_mutex_t, *my_pthread_mutex_ptr;


#define pthread_create my_pthread_create
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int my_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

#define pthread_exit my_pthread_exit
// void pthread_exit(void *retval);
void my_pthread_exit(void *retval);

#define pthread_join my_pthread_join
// int pthread_join(pthread_t thread, void **retval);
int my_pthread_join(pthread_t thread, void **retval);

// #undef pthread_mutexattr_t
// typedef void *pthread_mutexattr_t;  // ALWAYS NULL

#define pthread_mutex_init my_pthread_mutex_init
// int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int my_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

#define pthread_mutex_lock my_pthread_mutex_lock
// int pthread_mutex_lock(pthread_mutex_t *mutex);
int my_pthread_mutex_lock(pthread_mutex_t *mutex);

#define pthread_mutex_unlock my_pthread_mutex_unlock
// int pthread_mutex_unlock(pthread_mutex_t *mutex);
int my_pthread_mutex_unlock(pthread_mutex_t *mutex);

#define pthread_mutex_destroy my_pthread_mutex_destroy
// int pthread_mutex_destroy(pthread_mutex_t *mutex);
int my_pthread_mutex_destroy(pthread_mutex_t *mutex);

#include "queue.h"

#endif // USE_MY_PTHREAD
#endif // MY_PTHREAD_H
