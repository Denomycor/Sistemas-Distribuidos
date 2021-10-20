
#include "data.h"
#include "entry.h"
#include <string.h>
#include "client_stub-private.h"
#include "sdmessage.pb-c.h"
#include "serialization.h"
#include "priv-func.h"


/* Função para estabelecer uma associação entre o cliente e o servidor, 
 * em que address_port é uma string no formato <hostname>:<port>.
 * Retorna NULL em caso de erro.
 */
struct rtable_t *rtable_connect(const char *address_port){
    struct rtable_t*const tcp_socket = malloc(sizeof(struct rtable_t));
    char has_dest = 0;
    for(int i=0; i<strlen(address_port);i++){
        if(address_port[i] == ':'){
            tcp_socket->port = atoi(address_port+i+1);
            tcp_socket->ip = malloc(sizeof(char)*(i+1));
            memcpy(tcp_socket->ip, address_port, sizeof(char)*(i+1));
            ((char*)tcp_socket->ip)[i] = '\0';
            has_dest = 1; 
        }
    }

    tcp_socket->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket->sockfd == -1 || !has_dest){
        return NULL;
    }

    tcp_socket->socket.sin_family = AF_INET;
    tcp_socket->socket.sin_port = tcp_socket->port;
    if(inet_pton(AF_INET, tcp_socket->ip, &tcp_socket->socket.sin_addr) < 1){
        close(tcp_socket->sockfd);
        return NULL;
    }

    if (connect(tcp_socket->sockfd,(struct sockaddr *)&tcp_socket->socket, sizeof(tcp_socket->socket)) < 0) {
        close(tcp_socket->sockfd);
        return NULL;
    }

    return tcp_socket;
}

/* Termina a associação entre o cliente e o servidor, fechando a 
 * ligação com o servidor e libertando toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtable_disconnect(struct rtable_t *rtable){
    int r = close(rtable->sockfd);
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
    msg.data_size = entry_to_buffer(entry, &msg.data);
    //Send message
    int size = message_t__get_packed_size(&msg);
    void* buf = malloc(size);
    if(buf == NULL){
        return -1;
    }
    message_t__pack(&msg, buf);
    if((write(rtable->sockfd,buf,size)) != size){
        return -1;
    }
    free(buf); 
    free(msg.data);
    //Waiting for response
    size = MAX_BUF_SIZE; 
    buf = malloc(size);
    size = read(rtable->sockfd, buf, size);
    //Handling response
    MessageT* resp = message_t__unpack(NULL, size, buf);
    if(resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR){
        return -1;
    }
    message_t__free_unpacked(resp, NULL);
    free(buf);
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
    msg.data_size = strlen(key)+1; //has to be null terminated
    msg.data = key;
    //Send message
    int size = message_t__get_packed_size(&msg);
    void* buf = malloc(size);
    if(buf == NULL){
        return NULL;
    }
    message_t__pack(&msg, buf);
    if((write(rtable->sockfd,buf,size)) != size){
        return NULL;
    }
    free(buf); 
    //Waiting for response
    //free(msg.data);
    size = MAX_BUF_SIZE; 
    buf = malloc(size);
    size = read(rtable->sockfd,buf, size);
    //Handling response
    MessageT* resp = message_t__unpack(NULL, size, buf);
    if(resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR){
        return NULL;
    }
    struct data_t*const data = buffer_to_data(resp->data, resp->data_size);
    message_t__free_unpacked(resp, NULL);
    free(buf);
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
    msg.data_size = strlen(key)+1; //has to be null terminated
    msg.data = key;
    //Sends message
    int size = message_t__get_packed_size(&msg);
    void* buf = malloc(size);
    if(buf == NULL){
        return -1;
    }
    message_t__pack(&msg, buf);
    if((write(rtable->sockfd,buf,size)) != size){
        return -1;
    }
    free(buf); 
    //Waiting for response
    //free(msg.data);
    size = MAX_BUF_SIZE; 
    buf = malloc(size);
    size = read(rtable->sockfd, buf, size);
    //Handling response
    MessageT* resp = message_t__unpack(NULL, size, buf);
    if(resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR){
        return -1;
    }
    message_t__free_unpacked(resp, NULL);
    free(buf);
    return 0;
}

/* Devolve o número de elementos contidos na tabela.
 */
int rtable_size(struct rtable_t *rtable){
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_SIZE;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.data_size = 0;
    msg.data = NULL;
    //Send message
    int size = message_t__get_packed_size(&msg);
    void* buf = malloc(size);
    if(buf == NULL){
        return -1;
    }
    message_t__pack(&msg, buf);
    if((write(rtable->sockfd,buf,size)) != size){
        return -1;
    }
    //Waiting for response
    free(buf); 
    size = MAX_BUF_SIZE; 
    buf = malloc(size);
    size = read(rtable->sockfd, buf, size);
    //Handling response
    MessageT* resp = message_t__unpack(NULL, size, buf);
    if(resp==NULL){
        return -1;
    }
    int result = *((int*)resp->data);
    message_t__free_unpacked(resp, NULL);
    free(buf);
    return result;
}

/* Devolve um array de char* com a cópia de todas as keys da tabela,
 * colocando um último elemento a NULL.
 */
char **rtable_get_keys(struct rtable_t *rtable){
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.data_size = 0;
    msg.data = NULL;
    //Send msg
    int size = message_t__get_packed_size(&msg);
    void* buf = malloc(size);
    if(buf == NULL){
        return -1;
    }
    message_t__pack(&msg,buf);
    if((write(rtable->sockfd,buf,size)) != size){
        return -1;
    }
    free(buf);
    //waiting for response
    size = MAX_BUF_SIZE;
    buf = malloc(size);
    size = read(rtable->sockfd,buf,size);\
    //handling response
    MessageT* resp = message_t__unpack(NULL, size, buf);
    if(resp == NULL){
        return -1;
    }
    char** result = malloc((resp->data_size+1)*sizeof(char*)); //data_size = nº keys
    char* keys = malloc(strlen(resp->data)+1);
    strcpy(resp->data,keys);
    for(int i = 0; i < resp->data_size; i++){
        int length = 1;
        while(*(keys++)!='\0'){
            length++;
        }
        result[i] = subsstr(keys,0,length);
        keys = subsstr(keys,length,strlen(keys)+1);
    }
    result[resp->data_size+1] = NULL;
    free(keys);
    message_t__free_unpacked(resp, NULL);
    return result;

}

/* Liberta a memória alocada por rtable_get_keys().
 */
void rtable_free_keys(char **keys){
    for(int i=0; keys[i]!=NULL; i++){
        free(keys[i]);
    }
    free(keys);
}

/* Função que imprime o conteúdo da tabela remota para o terminal.
 */
void rtable_print(struct rtable_t *rtable) {
    MessageT msg;
    msg.opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;
    msg.data_size = 0;
    msg.data = NULL;
    //Send message
    int size = message_t__get_packed_size(&msg);
    void* buf = malloc(size);
    if(buf == NULL) {
        return -1;
    }
    message_t__pack(&msg, buf);
    if(write(rtable->sockfd,buf,size) != size) {
        return -1;
    }
    free(buf);
    //Waiting for response
    size = MAX_BUF_SIZE;
    buf = malloc(size);
    size = read(rtable->sockfd,buf,size);
    //Handling response
    MessageT* resp = message_t__unpack(NULL, size, buf);
    if(resp == NULL){
        return -1;
    }
    
}

