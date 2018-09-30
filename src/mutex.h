#ifndef MY_PTHREAD_MUTEX_H
#define MY_PTHREAD_MUTEX_H

#ifdef USE_MY_PTHREAD
#define _XOPEN_SOURCE  // OSX compatibility

#define pthread_mutexattr_t void
#define pthread_mutex_t my_pthread_mutex_t

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

#endif /* ifdef USE_MY_PTHREAD */

#endif /* ifndef MY_PTHREAD_MUTEX_H */
