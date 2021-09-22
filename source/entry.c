#include "include/entry.h"
#include "include/data.h"
#include <stdlib.h>
#include <string.h>


/* Função que cria uma entry, reservando a memória necessária e
 * inicializando-a com a string e o bloco de dados passados.
 */
struct entry_t *entry_create(char *key, struct data_t *data){
    struct entry_t* const temp = malloc(sizeof(struct entry_t));
    temp->key = key;
    temp->value = data;
    return temp;
}

/* Função que inicializa os elementos de uma entrada na tabela com o
 * valor NULL.
 */
void entry_initialize(struct entry_t* entry){
    entry->key = NULL;
    entry->value = NULL;
}

/* Função que elimina uma entry, libertando a memória por ela ocupada
 */
void entry_destroy(struct entry_t *entry){
    free(entry->key);
    data_destroy(entry->value);
    free(entry);
}

/* Função que duplica uma entry, reservando a memória necessária para a
 * nova estrutura.
 */
struct entry_t* entry_dup(struct entry_t *entry){
    struct entry_t *const temp = malloc(sizeof(struct entry_t));
    int len = strlen(entry->key)+1;
    temp->key = malloc(len);
    memcpy(temp->key, entry->key, len);
    temp->value = data_dup(entry->value);
    return temp;
}

/* Função que substitui o conteúdo de uma entrada entry_t.
*  Deve assegurar que destroi o conteúdo antigo da mesma.
*/
void entry_replace(struct entry_t* entry, char* new_key, struct data_t* new_value){
    free(entry->key);
    data_destroy(entry->value);
    entry->key = new_key;
    entry->value = new_value;
}

/* Função que compara duas entradas e retorna a ordem das mesmas.
*  Ordem das entradas é definida pela ordem das suas chaves.
*  A função devolve 0 se forem iguais, -1 se entry1<entry2, e 1 caso contrário.
*/
int entry_compare(struct entry_t* entry1, struct entry_t* entry2){
    int comp = strcmp(entry1->key, entry2->key);
    return comp<=0?comp<0?-1:0:1;
}