/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "message/serialization.h"
#include <stdlib.h>
#include <string.h>

/* Serializa uma estrutura data num buffer que será alocado
 * dentro da função. Além disso, retorna o tamanho do buffer
 * alocado ou -1 em caso de erro.
 */
int data_to_buffer(struct data_t *data, char **data_buf) {
    if (data == NULL || data_buf == NULL) {
        return -1;
    }
    
    int size = sizeof(int) + data->datasize;
    char *const temp = malloc(size);
    if (temp == NULL) {
        return -1;
    }
    *temp = data->datasize;
    memcpy(temp+sizeof(int), data->data, data->datasize);
    *data_buf = temp;
    return size;
}

/* De-serializa a mensagem contida em data_buf, com tamanho
 * data_buf_size, colocando-a e retornando-a numa struct
 * data_t, cujo espaco em memoria deve ser reservado.
 * Devolve NULL em caso de erro.
 */
struct data_t *buffer_to_data(char *data_buf, int data_buf_size) {
    if (data_buf == NULL || data_buf_size == -1) {
        return NULL;
    }
    int size = *(data_buf);
    void *data = malloc(size);
    memcpy(data, data_buf+sizeof(int), size);
    return data_create2(size, data);
}

/* Serializa uma estrutura entry num buffer que sera alocado
 * dentro da função. Além disso, retorna o tamanho deste
 * buffer alocado ou -1 em caso de erro.
 */
int entry_to_buffer(struct entry_t *data, char **entry_buf) {
    if (data == NULL || entry_buf == NULL) {
        return -1;
    }

    int ksize = strlen(data->key) + 1;

    int size = 2 * sizeof(int) + ksize + data->value->datasize;

    char *const temp = malloc(size);

    memset(temp, 0, size);
    *temp = ksize;
    *(temp + sizeof(int)) = data->value->datasize;
    memcpy(temp + 2*sizeof(int), data->key, ksize);
    memcpy(temp + 2*sizeof(int) + ksize, data->value->data, data->value->datasize);
    
    *entry_buf =  temp;
    return size;
}

/* De-serializa a mensagem contida em entry_buf, com tamanho
 * entry_buf_size, colocando-a e retornando-a numa struct
 * entry_t, cujo espaco em memoria deve ser reservado.
 * Devolve NULL em caso de erro.
 */
struct entry_t *buffer_to_entry(char *entry_buf, int entry_buf_size) {
    if (entry_buf == NULL || entry_buf_size == -1) {
        return NULL;
    }
    
    int ksize = *entry_buf;
    int dsize = *(entry_buf + sizeof(int));

    void *data = malloc(dsize);
    memcpy(data, entry_buf + 2*sizeof(int) + ksize, dsize);

    char *key = malloc(ksize);
    memcpy(key, entry_buf + 2*sizeof(int), ksize);

    struct data_t *temp = data_create2(dsize , data);
    struct entry_t *temp2 = entry_create(key, temp);

    return temp2;
}
