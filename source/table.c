#include "include\table.h"
#include "include\table-private.h"
#include "include\list-private.h"
#include "include\data.h"
#include "include\list.h"
#include "include\entry.h"
#include <stdlib.h>
#include <string.h>

/* Função para criar/inicializar uma nova tabela hash, com n
 * linhas (n = módulo da função hash)
 * Em caso de erro retorna NULL.
 */
struct table_t* table_create(int n){
    struct table_t* temp = malloc(sizeof(struct table_t));
    temp->n = n;
    temp->lists_ptr = malloc(sizeof(struct list_t*)*n);
    for(int i=0; i<n; i++){
        temp->lists_ptr[i] = list_create();
    }
    return temp;
}

/* Função para libertar toda a memória ocupada por uma tabela.
 */
void table_destroy(struct table_t* table){
    for(int i=0; i<table->n;i++){
        list_destroy(table->lists_ptr[i]);
    }
    free(table->lists_ptr);
    free(table);
}

/* Função para adicionar um par chave-valor à tabela.
 * Os dados de entrada desta função deverão ser copiados, ou seja, a
 * função vai *COPIAR* a key (string) e os dados para um novo espaço de
 * memória que tem de ser reservado. Se a key já existir na tabela,
 * a função tem de substituir a entrada existente pela nova, fazendo
 * a necessária gestão da memória para armazenar os novos dados.
 * Retorna 0 (ok) ou -1 em caso de erro.
 */
int table_put(struct table_t *table, char *key, struct data_t *value){
    int index = get_hash_index(key, table->n);
    struct entry_t* const temp = entry_create(strdup(key), data_dup(value));
    list_add(table->lists_ptr[index], temp);
    //TODO: Returning feedback to the client
    //TODO: Replace if already exists
}

/* Função para obter da tabela o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser
 * libertados no contexto da função que chamou table_get, ou seja, a
 * função aloca memória para armazenar uma *CÓPIA* dos dados da tabela,
 * retorna o endereço desta memória com a cópia dos dados, assumindo-se
 * que esta memória será depois libertada pelo programa que chamou
 * a função.
 * Devolve NULL em caso de erro.
 */
struct data_t *table_get(struct table_t *table, char *key);

/* Função para remover um elemento da tabela, indicado pela chave key, 
 * libertando toda a memória alocada na respetiva operação table_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int table_del(struct table_t *table, char *key);

/* Função que devolve o número de elementos contidos na tabela.
 */
int table_size(struct table_t *table);

/* Função que devolve um array de char* com a cópia de todas as keys da
 * tabela, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
char **table_get_keys(struct table_t *table);

/* Função que liberta toda a memória alocada por table_get_keys().
 */
void table_free_keys(char **keys);

/* Função que imprime o conteúdo da tabela.
 */
void table_print(struct table_t *table);