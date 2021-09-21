#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"

struct node_t {
   
};

struct list_t {
    struct entry_t* value;
    struct list_t* next;
};

void list_print(struct list_t* list);

#endif
