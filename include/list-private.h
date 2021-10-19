/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"

/* Estrutura que define o nó de uma lista. 
*/
struct node_t {
   struct entry_t* entry; /* Conteúdo guardado no nó de uma lista do tipo entry */
   struct node_t* next; /* Pointer para o próximo elemento da lista */
};

/* Estrutura que define uma lista ligada.
*/
struct list_t {
    struct node_t* head; /* Cabeça da lista */
    struct node_t* tail;/* Cauda da lista */
};

/*Função que obtém da lista o node que contém a entry com a chave key.
* Retorna a referência do node ou NULL em caso de erro.
*/
struct node_t *list_get_node(struct list_t *list, char *key);

#endif
