#include "data.h"
#include <stdlib.h>
#include <string.h>

/* Função que cria um novo elemento de dados data_t e reserva a memória
 * necessária, especificada pelo parâmetro size 
 */
struct data_t *data_create(int size) {
    if (size < 1) {
        return NULL;
    }
    
    struct data_t *const temp = malloc(sizeof(struct data_t));
    temp->datasize = size;
    temp->data = malloc(size);
    return temp;
}

/* Função idêntica à anterior, mas que inicializa os dados de acordo com
 * o parâmetro data.
 */
struct data_t *data_create2(int size, void *data) {
    if (size < 1 || data == NULL) {
        return NULL;
    }

    struct data_t *const temp = malloc(sizeof(struct data_t));
    temp->datasize = size;
    temp->data = data;
    return temp;
}

/* Função que elimina um bloco de dados, apontado pelo parâmetro data,
 * libertando toda a memória por ele ocupada.
 */
void data_destroy(struct data_t *data) {
    if (data == NULL) {
        return;
    }
    if (data->data != NULL) {
        free(data->data);
    }
    free(data);
}

/* Função que duplica uma estrutura data_t, reservando a memória
 * necessária para a nova estrutura.
 */
struct data_t *data_dup(struct data_t *data) {
    if (data == NULL || data->data == NULL ) {
        return NULL;
    }
    struct data_t *const temp = data_create(data->datasize);
    if (temp != NULL) {
        memcpy(temp->data, data->data, data->datasize);
    }
    return temp;
}

/* Função que substitui o conteúdo de um elemento de dados data_t.
*  Deve assegurar que destroi o conteúdo antigo do mesmo.
*/
void data_replace(struct data_t *data, int new_size, void *new_data) {
    free(data->data);
    data->datasize = new_size;
    data->data = new_data;
}

//Revisto, nao ha copias desnecessarias, buffers sao movidos e apaga-se a si proprio, bug free