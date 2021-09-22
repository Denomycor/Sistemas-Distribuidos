#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list.h"

struct table_t {
    struct list_t** list_refs;
    char** keys;
};

int hash(const char* key ,int n){
    int counter = 0;
    for(int i=0; key[i]!='\0'; i++){
        counter += key[i];
    }
    return counter % n;
}


#endif
