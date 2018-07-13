#define DEBUG_ON 1
#define PADDING 32
#include "dbg_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>


void *dbg_malloc(size_t size){
    void *p = malloc(size+2 * PADDING);
    if(!p) return NULL;
    memset(p, 0xEE, size+2 * PADDING);
    *(((size_t *) p) + 1) = size;
    return p+PADDING;
}

void *dbg_realloc(void *p, size_t new_size){
    void *actual_p, *new_p;
    size_t old_size;
    actual_p = p - PADDING;
    old_size = *(((size_t*) actual_p) + 1);
    new_p = realloc(actual_p, new_size + 2 * PADDING);
    if(!new_p) return NULL;
    *(((size_t *)new_p) + 1) = new_size;
    if(new_size > old_size){
        memset((new_p + old_size + 2 * PADDING), 0xEE, new_size - old_size);
    }
    return new_p + PADDING;
}
void dbg_free(void *p){
    void *actual_p;
    size_t size;
    int i;
    if(!p) return;
    actual_p = p - PADDING;
    size = *(((size_t *)actual_p) + 1);
    *(((size_t *)actual_p) + 1) = 0xEEEEEEEEEEEEEEEE;
    for(i = 0; i < PADDING; i++){
        if( *(((char *)actual_p) + i) != ((char)0xEE)){
            printf("Corrupted on the first %d byte of pointer %p, at beginning\n", i, p);
            raise(11);
        }
    }
    for(i = 0; i < PADDING; i++){
        if( *(((char *)p) + i + size) != ((char)0xEE)){
            printf("Corrupted on the first %d byte of pointer %p, at end\n", i, p);
            raise(11);
        }
    }
    free(actual_p);
}
