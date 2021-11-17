/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "list-private.h"
#include <stdlib.h>
#include <string.h>


/*Função que obtém da lista o node que contém a entry com a chave key.
* Retorna a referência do node ou NULL em caso de erro.
*/
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
