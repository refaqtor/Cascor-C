/*	Memory Allocation Library

    Changed variable names for clarity

    v2.0
    Ian Chiu (ichiu@andrew.cmu.edu)
    7/17/18

*/

#include <stdio.h>
#include <stdlib.h>


/*	ALLOC MEM -  Allocates memory for 'Nitems' of 'itemSize'.  If the
    memory allocation fails, the function flames out, with an error
    messages stating what happened and who the caller was ('func_desc').

    Returns a void pointer to the allocated memory.
*/

void *alloc_mem ( int Nitems, int itemSize, char *func_desc )
{
    void *ptr;

    if  ( (ptr = malloc( Nitems * itemSize )) == NULL )  {
        fprintf ( stderr, "\nFATAL ERROR: Unable to allocate memory in %s\n\n",
                func_desc );
        exit( 1 );
    }

    return ptr;
}


/*	REALLOC MEM -  This function is essentially the same as alloc_mem,
    except that it allows the user to change the ammount of memory
    associated with 'ptr'.  If 'ptr' is NULL, then this function acts
    exactly like alloc_mem.  The contents of 'ptr' are guaranteed to
    remain intact in the newly allocated memory.
*/

void *realloc_mem ( void *ptr, int Nitems, int itemSize, char *func_desc )
{
    if  ( (ptr = realloc( ptr, Nitems * itemSize )) == NULL )  {
        fprintf ( stderr, "\nFATAL ERROR: Unable to reallocate memory in %s\n\n"
                  , func_desc );
        exit( 1 );
    }

    return ptr;
}


void *free_mem ( void *ptr )
{
    if  ( ptr != NULL )
        free( ptr );
    return NULL;
}
