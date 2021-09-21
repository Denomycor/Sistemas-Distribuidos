#include "include/data.h"
#include <stdlib.h>
#include <string.h>

/* Função que cria um novo elemento de dados data_t e reserva a memória
 * necessária, especificada pelo parâmetro size 
 */
struct data_t *data_create(int size){
    struct data_t* const temp = malloc(sizeof(struct data_t));
    temp->datasize = size;
    temp->data = malloc(size);
    return temp;
}

/* Função idêntica à anterior, mas que inicializa os dados de acordo com
 * o parâmetro data.
 */
struct data_t *data_create2(int size, void *data){
    struct data_t* const temp = malloc(sizeof(struct data_t));
    temp->datasize = size;
    temp->data = malloc(size);
    memcpy(temp->data, data, size);
    return temp;
}

/* Função que elimina um bloco de dados, apontado pelo parâmetro data,
 * libertando toda a memória por ele ocupada.
 */
void data_destroy(struct data_t *data){
    free(data->data);
    free(data);
}

/* Função que duplica uma estrutura data_t, reservando a memória
 * necessária para a nova estrutura.
 */
struct data_t *data_dup(struct data_t *data){
    struct data_t* const temp = malloc(sizeof(struct data_t));
    temp->datasize = data->datasize;
    temp->data = malloc(temp->datasize);
    memcpy(temp->data, data->data, temp->datasize);
    return temp;
}

/* Função que substitui o conteúdo de um elemento de dados data_t.
*  Deve assegurar que destroi o conteúdo antigo do mesmo.
*/
void data_replace(struct data_t *data, int new_size, void *new_data){
    free(data->data);
    data->datasize = new_size;
    data->data = malloc(new_size);
    memcpy(data->data, new_data, new_size);
}