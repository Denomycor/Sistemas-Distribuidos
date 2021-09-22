#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list.h"

struct table_t {
    struct list_t** lists_ptr; //Cant have an array of list_t because the function list_destroy would try to free the list address when its supposed to free the whole array
    int n;
};

int get_hash_index(const char* key ,int n);

#endif
