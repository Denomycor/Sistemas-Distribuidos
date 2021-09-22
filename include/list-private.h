#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"

struct node_t {
   struct entry_t* entry;
   struct node_t* next;
};

struct list_t {
    struct node_t* head;
    struct node_t* tail;
};

//TODO
void list_print(struct list_t* list);

#endif
