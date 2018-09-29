#ifndef MY_PTHREAD_H
#define MY_PTHREAD_H

#define USE_MY_PTHREAD 1
#ifdef USE_MY_PTHREAD

#include <sys/ucontext.h>

#define pthread_t my_pthread_t
#define pthread_ptr my_pthread_ptr
typedef struct my_pthread_t {
  ucontext_t *uc_link;
  my_pthread_t *next;
} *my_pthread_ptr;


typedef void *pthread_attr_t;  // ALWAYS NULL

#define pthread_mutex_t my_pthread_mutex_t
#define pthread_mutex_ptr my_pthread_mutex_ptr
typedef struct my_pthread_mutex_t {
  ucontext_t *owner;
  my_pthread_mutex_t *next;
} *my_pthread_mutex_ptr;


#define pthread_create my_pthread_create
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int my_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

#define pthread_exit my_pthread_exit
// void pthread_exit(void *retval);
void my_pthread_exit(void *retval);

#define pthread_join my_pthread_join
// int pthread_join(pthread_t thread, void **retval);
int my_pthread_join(pthread_t thread, void **retval);

typedef void *pthread_mutexattr_t;  // ALWAYS NULL
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




#endif // USE_MY_PTHREAD
#endif // MY_PTHREAD_H
