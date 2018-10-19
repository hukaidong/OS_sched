
#define LIBRARYREQ 0x01
#define THREADREQ ~LIBRARYREQ


#define malloc(x) myallocate(x, __FILE__, __LINE__, THREADREQ)
#define free(x) mydeallocate(x, __FILE__, __LINE__, THREADREQ)

#define _lib_malloc(x) myallocate(x, __FILE__, __LINE__, LIBRARYREQ)
#define _lib_free(x) mydeallocate(x, __FILE__, __LINE__, LIBRARYREQ)

#define UNUSED(x) (void)(x)
#define NULL 0

void *myallocate(int size, char *fname,int lnum, char flags);
void *mydeallocate(int size, char *fname, int line, char flags);
