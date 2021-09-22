#include "include\list.h"
#include "include\list-private.h"
#include <stdlib.h>
#include <string.h>


/* Função que cria uma nova lista (estrutura list_t a ser definida pelo
 * grupo no ficheiro list-private.h).
 * Em caso de erro, retorna NULL.
 */
struct list_t *list_create(){
    struct list_t* const temp = malloc(sizeof(struct list_t));
    if(temp == NULL) {
        return temp;
    }
    temp->head = malloc(sizeof(struct node_t));
    temp->tail = temp->head;
    return temp;
}

/* Função que elimina uma lista, libertando *toda* a memoria utilizada
 * pela lista.
 */
void list_destroy(struct list_t *list);

/* Função que adiciona no final da lista (tail) a entry passada como
* argumento caso não exista na lista uma entry com key igual àquela
* que queremos inserir.
* Caso exista, os dados da entry (value) já existente na lista serão
* substituídos pelos os da nova entry.
* Retorna 0 (OK) ou -1 (erro).
*/

int list_add(struct list_t *list, struct entry_t *entry);

/* Função que elimina da lista a entry com a chave key.
 * Retorna 0 (OK) ou -1 (erro).
 */
int list_remove(struct list_t *list, char *key) {
    struct node* iter = list->head;
    while(iter != NULL) {

    }
    return -1;
}

/* Função que obtém da lista a entry com a chave key.
 * Retorna a referência da entry na lista ou NULL em caso de erro.
 * Obs: as funções list_remove e list_destroy vão libertar a memória
 * ocupada pela entry ou lista, significando que é retornado NULL
 * quando é pretendido o acesso a uma entry inexistente.
*/
struct entry_t *list_get(struct list_t *list, char *key) {
    struct node_t* iter = list->head;
    while(iter != NULL) {
        if(strcmp(iter->entry->key, key) == 0) {
            return iter->entry;
        }
        iter = iter->next;
    }
    return NULL;
}

/* Função que retorna o tamanho (número de elementos (entries)) da lista,
 * ou -1 (erro).
 */
int list_size(struct list_t *list) {
    int counter = 0;
    struct node_t* iter = list->head;
    while(iter != NULL) {
        counter++;
        iter = iter->next;
    }
    return counter;
}

/* Função que devolve um array de char* com a cópia de todas as keys da 
 * tabela, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
char **list_get_keys(struct list_t *list);

/* Função que liberta a memória ocupada pelo array das keys da tabela,
 * obtido pela função list_get_keys.
 */
void list_free_keys(char **keys);

/* Função que imprime o conteúdo da lista para o terminal.
 */
void list_print(struct list_t *list);
