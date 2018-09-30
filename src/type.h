#ifndef MY_PTHREAD_TYPE_H
#define MY_PTHREAD_TYPE_H

typedef ucontext* ucxt_p;

define_list(uckt_p);

typedef List(ucxt_p)* Qucxt;

typedef struct {
  ucxt_p  uc_link;
  void    *rval;
  ucxt_p  to_join;
} my_pthread_t;

typedef struct {
  // Memory allignment
  int     locked;
  Qucxt   pending;
} my_pthread_mutex_t;

#endif /* ifndef MY_PTHREAD_TYPE_H */
