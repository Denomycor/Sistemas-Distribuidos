#include "server/access_man.h"


int rw_mutex_init(rw_mutex_t* m){
    int res;
    res = pthread_mutex_init(&m->control_mutex, NULL);
    if(res != 0)
        return res;
    res = pthread_cond_init(&m->cond, NULL);
    if(res != 0)
        return res;
    return 0;
}

int read_exclusive_lock(rw_mutex_t* m){
    int res;
    res = pthread_mutex_lock(&m->control_mutex);
    if(res != 0)
        return res;
    while(m->wc>0){
        pthread_cond_wait(&m->cond, &m->control_mutex);
    }
    m->rc++;
    pthread_mutex_unlock(&m->control_mutex);
    return 0;
}

int write_exclusive_lock(rw_mutex_t* m){
    int res;
    res = pthread_mutex_lock(&m->control_mutex);
    if(res != 0)
        return res;
    while(m->wc>0 || m->rc>0){
        pthread_cond_wait(&m->cond, &m->control_mutex);
    }
    m->wc++;
    pthread_mutex_unlock(&m->control_mutex);
    return 0;
}


int read_exclusive_unlock(rw_mutex_t* m){
    int res;
    res = pthread_mutex_lock(&m->control_mutex);
    if(res!=0)
        return res;
    m->rc--;
	pthread_cond_broadcast(&m->cond);
	pthread_mutex_unlock(&m->control_mutex);
    return 0;
}

int write_exclusive_unlock(rw_mutex_t* m){
    int res;
    res = pthread_mutex_lock(&m->control_mutex);
    if(res!=0)
        return res;
    m->wc--;
	pthread_cond_broadcast(&m->cond);
	pthread_mutex_unlock(&m->control_mutex);
    return 0;
}

int rw_mutex_destroy(rw_mutex_t* m){
    int res;
    res = pthread_mutex_destroy(&m->control_mutex);
    if(res != 0)
        return res;
    res = pthread_cond_destroy(&m->cond);
    if(res != 0)
        return res;
    return 0;
}
