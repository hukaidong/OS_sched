#ifndef MY_PTHREAD_TYPE_H
#define MY_PTHREAD_TYPE_H

typedef ucontext* ucxt_p;
define_list(uckt_p);
typedef List(ucxt_p)* Qucxt;
typedef List(ucxt_p)* my_pthread_mutex_t;

#endif /* ifndef MY_PTHREAD_TYPE_H */
