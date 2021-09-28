/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

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

/*Função que obtém da lista o node que contém a entry com a chave key.
* Retorna a referência do node ou NULL em caso de erro.
*/
struct node_t *list_get_node(struct list_t *list, char *key);

#endif
