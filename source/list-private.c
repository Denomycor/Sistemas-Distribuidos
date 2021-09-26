#include "list-private.h"
#include <stdlib.h>
#include <string.h>

struct node_t *list_get_node(struct list_t *list, char *key) {
    struct node_t* iter = list->head;
    while(iter != NULL) {
        if(strcmp(iter->entry->key, key) == 0) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}
