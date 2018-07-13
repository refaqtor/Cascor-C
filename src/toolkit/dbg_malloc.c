#define DEBUG_ON 1
#include "dbg_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>

void *dbg_malloc(size_t size){
    void *p = malloc(size+2000);
    memset(p, 0xEE, size+2000);
    *(((size_t *) p) + 1) = size;
    return p+1000;
}

void *dbg_realloc(void *p, size_t new_size){
    void *actual_p, *new_p;
    size_t old_size;
    actual_p = p - 1000;
    old_size = *(((size_t*) actual_p) + 1);
    new_p = realloc(actual_p, new_size);
    *(((size_t *)new_p) + 1) = new_size;
    memset((new_p + old_size + 2000), 0xEE, new_size - old_size);
    return new_p + 1000;
}

void dbg_free(void *p){
    void *actual_p;
    size_t size;
    int i;
    if(!p) return;
    actual_p = p - 1000;
    *(((size_t *)p) + 1) = 0xEEEEEEEEEEEEEEEE;
    for(i = 0; i < 1000; i++){
        if( *(((char *)p) + i) != 0xEE){
            printf("Corrupted on the first %d byte of pointer %p", i, p);
            raise(11);
        }
    }
    for(i = 0; i < 1000; i++){
        if( *(((char *)p) + i + size) != 0xEE){
            printf("Corrupted on the first %d byte of pointer %p", i, p);
            raise(11);
        }
    }
    free(actual_p);
}

