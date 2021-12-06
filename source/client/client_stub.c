/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "client/client_stub-private.h"
#include "client/client_stub.h"
#include "client/network_client.h"
#include "message/serialization.h"
#include "helper/priv-func.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* Função para estabelecer uma associação entre o cliente e o servidor, 
 * em que address_port é uma string no formato <hostname>:<port>.
 * Retorna NULL em caso de erro.
 */
struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t*const tcp_socket = malloc(sizeof(struct rtable_t));
    if(tcp_socket==NULL){
        return NULL;
    }
    tcp_socket->ip = NULL;
    tcp_socket->port = -1;

    if(-1 == parse_address(address_port, &tcp_socket->ip, &tcp_socket->port)){
        printf("ERROR! - Incorrect address format");
    }

    tcp_socket->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket->sockfd == -1 || tcp_socket->ip == NULL || tcp_socket->port == -1){
        return NULL;
    }

    if(network_connect(tcp_socket) == -1) {
        return NULL;
    }
    return tcp_socket;
}

/* Termina a associação entre o cliente e o servidor, fechando a 
 * ligação com o servidor e libertando toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtable_disconnect(struct rtable_t *rtable){
    int r = network_close(rtable);
    free(rtable->ip);
    free(rtable);
    return r;
}

/* Função para adicionar um elemento na tabela.
 * Se a key já existe, vai substituir essa entrada pelos novos dados.
 * Devolve 0 (ok, em adição/substituição) ou -1 (problemas).
 */
int rtable_put(struct rtable_t *rtable, struct entry_t *entry){
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg.c_type = MESSAGE_T__C_TYPE__CT_ENTRY;
    msg.buffer.len = entry_to_buffer(entry, (char**)&msg.buffer.data);

    MessageT* resp = network_send_receive(rtable, &msg);

    if(resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR){
        message_t__free_unpacked(resp, NULL);
        return -1;
    }
    message_t__free_unpacked(resp, NULL);

    return 0; 
}

/* Função para obter um elemento da tabela.
 * Em caso de erro, devolve NULL.
 */
struct data_t *rtable_get(struct rtable_t *rtable, char *key){
    //uses key, does not copy it
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GET;
    msg.c_type = MESSAGE_T__C_TYPE__CT_KEY;
    msg.buffer.len = strlen(key)+1; //has to be null terminated
    msg.buffer.data = key;

    MessageT* resp = network_send_receive(rtable, &msg);

    if(resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR){
        message_t__free_unpacked(resp, NULL);
        return NULL;
    }

    struct data_t* const data = buffer_to_data(resp->buffer.data, resp->buffer.len);

    message_t__free_unpacked(resp, NULL);

    return data;
}

/* Função para remover um elemento da tabela. Vai libertar 
 * toda a memoria alocada na respetiva operação rtable_put().
 * Devolve: 0 (ok), -1 (key not found ou problemas).
 */
int rtable_del(struct rtable_t *rtable, char *key){
    //uses key, does not copy it
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_DEL;
    msg.c_type = MESSAGE_T__C_TYPE__CT_KEY;
    msg.buffer.len = strlen(key)+1; //has to be null terminated
    msg.buffer.data = key;

    MessageT* resp = network_send_receive(rtable, &msg);

    if(resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR){
        message_t__free_unpacked(resp, NULL);
        return -1;
    }
    message_t__free_unpacked(resp, NULL);

    return 0;
}

/* Devolve o número de elementos contidos na tabela.
 */
int rtable_size(struct rtable_t *rtable){
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_SIZE;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.buffer.len = 0;
    msg.buffer.data = NULL;

    MessageT* resp = network_send_receive(rtable, &msg);

    if(resp==NULL){
        message_t__free_unpacked(resp, NULL);
        return -1;
    }

    int result = *(resp->buffer.data);

    message_t__free_unpacked(resp, NULL);
    return result;
}

/* Obtém as estatísticas do servidor */
struct statistics* rtable_stats(struct rtable_t *rtable){
    MessageT msg;
    message_t__init(&msg);  
    msg.opcode = MESSAGE_T__OPCODE__OP_STATS;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.buffer.len = 0;
    msg.buffer.data = NULL;

    MessageT* resp = network_send_receive(rtable, &msg);

    if(resp == NULL){
        message_t__free_unpacked(resp, NULL);
        return NULL;
    }

    struct statistics* const stats = (struct statistics*)resp->buffer.data;
    resp->buffer.data = NULL;
    resp->buffer.len = 0;

    message_t__free_unpacked(resp, NULL);
    return stats;
}

/* Devolve um array de char* com a cópia de todas as keys da tabela,
 * colocando um último elemento a NULL.
 */
char **rtable_get_keys(struct rtable_t *rtable){
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.buffer.len = 0;
    msg.buffer.data = NULL;

    MessageT* resp = network_send_receive(rtable, &msg);

    if(resp == NULL){
        message_t__free_unpacked(resp, NULL);
        return NULL;
    }

    int nkeys = 0;
    for(char* i = resp->buffer.data; i < (char*)resp->buffer.data+resp->buffer.len; i++){
        if(*i == '\0'){
            nkeys++;
        }
    }
    char** result = malloc((nkeys+1)*sizeof(char*)); 
    char* keys = resp->buffer.data;
    resp->buffer.data = NULL;
    resp->buffer.len = 0;
    char* keyref = keys;
    for(int i = 0; i < nkeys; i++){
        result[i] = keyref;
        keyref += strlen(keyref)+1;
    }
    result[nkeys] = NULL;

    //Free response
    message_t__free_unpacked(resp, NULL);
    return result;

}

/* Liberta a memória alocada por rtable_get_keys().
 */
void rtable_free_keys(char **keys){
    free(*keys);
    free(keys);
}


/* Função que imprime o conteúdo da tabela remota para o terminal.
 */
void rtable_print(struct rtable_t *rtable) {
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.buffer.len = 0;
    msg.buffer.data = NULL;
    
    MessageT* rsp = network_send_receive(rtable, &msg);

    printf("%s", rsp->buffer.data);

    message_t__free_unpacked(rsp,NULL);
}
