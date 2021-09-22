#include "include\table.h"
#include "include\data.h"
#include "include\list.h"
#include <stdlib.h>

/* Função para criar/inicializar uma nova tabela hash, com n
 * linhas (n = módulo da função hash)
 * Em caso de erro retorna NULL.
 */
struct table_t* table_create(int n){
    struct table_t** temp = malloc(sizeof(struct table_t*)*n);
    for(int i=0; i<n; i++){
        temp[i] = list_create();
    }
    return temp;
}