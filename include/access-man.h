#ifndef _ACCESS_MAN_H
#define _ACCESS_MAN_H

#include <stdlib.h>
#include <pthread.h>

typedef struct {
    size_t rc, wc;
    pthread_mutex_t mutex;
} rw_exc_t;

int rw_exc_init(rw_exc_t* m);

int read_exclusive_lock(rw_exc_t* m);
int read_exc(rw_exc_t* m);

int read_exclusive_unlock(rw_exc_t* m);

int write_exclusive_lock(rw_exc_t* m);
int write_exc(rw_exc_t* m);

int write_exclusive_unlock(rw_exc_t* m);

int rw_exc_destroy(rw_exc_t* m);

#endif
