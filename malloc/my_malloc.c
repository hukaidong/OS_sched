#include "include/my_malloc.h"


void *myallocate(int size, char *fname,int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  UNUSED(flags);

  return NULL;
}
void *mydeallocate(int size, char *fname, int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  UNUSED(flags);

  return NULL;
}
