#include "server/access-man.h"


int rw_exc_init(rw_exc_t* m){
    m->rc=0;
    m->wc=0;
    return pthread_mutex_init(&m->mutex, NULL);
}

int read_exclusive_lock(rw_exc_t* m){
    int res;
    do{
        res = read_exc(m);
    }while(res!=0);
    return res;
}

int write_exclusive_lock(rw_exc_t* m){
    int res;
    do{
        res = write_exc(m);
    }while(res!=0);
    return res;
}

int read_exc(rw_exc_t* m){
    int err;
    if((err = pthread_mutex_lock(&m->mutex))!=0)
        return err;

    if(m->wc>0){
        if((err = pthread_mutex_unlock(&m->mutex))!=0)
            return err;

        return -1;
    }else{
        m->rc++;
        if((err = pthread_mutex_unlock(&m->mutex))!=0)
            return err;
        return 0;
    }
}

int write_exc(rw_exc_t* m){
    int err;
    if((err = pthread_mutex_lock(&m->mutex))!=0)
        return err;

    if(m->rc>0){
        if((err = pthread_mutex_unlock(&m->mutex))!=0)
            return err;

        return -1;
    }else{
        m->wc++;
        if((err = pthread_mutex_unlock(&m->mutex))!=0)
            return err;
        return 0;
    }
}

int read_exclusive_unlock(rw_exc_t* m){
    int err;
    if((err = pthread_mutex_lock(&m->mutex))!=0)
        return err;
    m->rc--;
    if((err = pthread_mutex_unlock(&m->mutex))!=0)
        return err;
    return 0;
}

int write_exclusive_unlock(rw_exc_t* m){
    int err;
    if((err = pthread_mutex_lock(&m->mutex))!=0)
        return err;
    m->wc--;
    if((err = pthread_mutex_unlock(&m->mutex))!=0)
        return err;
    return 0;
}

int rw_exc_destroy(rw_exc_t* m){
    return pthread_mutex_destroy(&m->mutex);
}