#ifndef _ACCESS_MAN_H
#define _ACCESS_MAN_H

#include <stdlib.h>
#include <pthread.h>


/* Struct thats defines a mutex thats never allows running read and
 * write operations at the same time, however, they can run at the same
 * time if they are both read or write
 */
typedef struct {
    size_t rc, wc;
    pthread_mutex_t mutex;
} rw_exc_t;

/* Initialize the passed memory, fowards pthread_mutex_init return value
 */
int rw_exc_init(rw_exc_t* m);

/* Locks a mutex for a function that is responsible for reading values,
 * fowards pthread_mutex_lock return value
 */
int read_exclusive_lock(rw_exc_t* m);
int read_exc(rw_exc_t* m);

/* Unlocks a mutex for a function that is responsible for reading values,
 * fowards pthread_mutex_unlock return value
 */
int read_exclusive_unlock(rw_exc_t* m);

/* Locks a mutex for a function that is responsible for writing values,
 * fowards pthread_mutex_lock return value
 */
int write_exclusive_lock(rw_exc_t* m);
int write_exc(rw_exc_t* m);

/* Unlocks a mutex for a function that is responsible for writing values,
 * fowards pthread_mutex_unlock return value
 */
int write_exclusive_unlock(rw_exc_t* m);

/* Destroys the passed struct, fowards pthread_mutex_destroy return value
 */
int rw_exc_destroy(rw_exc_t* m);

#endif
