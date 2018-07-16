#include <stdio.h>
/*
 * functionally works like malloc, realloc, and free,
 * but adds overhead to the beginning and end of each datablock
 * to check for memory corruption.
 */


void *dbg_malloc( size_t size );
void *dbg_realloc( void * p, size_t new_size );
void dbg_free( void *p );


#ifndef DEBUG_ON
#define malloc(X) dbg_malloc(X)
#define realloc(X,Y) dbg_realloc(X,Y)
#define free(X) dbg_free(X)
#endif
