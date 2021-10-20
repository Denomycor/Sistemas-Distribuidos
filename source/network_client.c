
#include "client_stub.h"
#include "sdmessage.pb-c.h"
#include "client_stub-private.h"

/* Esta função deve:
 * - Obter o endereço do servidor (struct sockaddr_in) a base da
 *   informação guardada na estrutura rtable;
 * - Estabelecer a ligação com o servidor;
 * - Guardar toda a informação necessária (e.g., descritor do socket)
 *   na estrutura rtable;
 * - Retornar 0 (OK) ou -1 (erro).
 */
int network_connect(struct rtable_t *rtable) {
    
    //Na minha opiniao isto devia estar aqui
    rtable->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(rtable->sockfd == -1){
        return -1;
    }

    rtable->socket.sin_family = AF_INET;
    rtable->socket.sin_port = rtable->port;
    if(inet_pton(AF_INET, rtable->ip, &rtable->socket.sin_addr) < 1){
        close(rtable->sockfd);
        return -1;
    }

    if (connect(rtable->sockfd,(struct sockaddr *)&rtable->socket, sizeof(rtable->socket)) < 0) {
        close(rtable->sockfd);
        return -1;
    }

    return 0;
}

/* Esta função deve:
 * - Obter o descritor da ligação (socket) da estrutura rtable_t;
 * - Reservar memória para serializar a mensagem contida em msg;
 * - Serializar a mensagem contida em msg;
 * - Enviar a mensagem serializada para o servidor;
 * - Libertar a memória ocupada pela mensagem serializada enviada;
 * - Esperar a resposta do servidor;
 * - Reservar a memória para a mensagem serializada recebida;
 * - De-serializar a mensagem de resposta, reservando a memória 
 *   necessária para a estrutura MessageT que é devolvida;
 * - Libertar a memória ocupada pela mensagem serializada recebida;
 * - Retornar a mensagem de-serializada ou NULL em caso de erro.
 */
MessageT *network_send_receive(struct rtable_t * rtable, MessageT *msg) {
    int len = message_t__get_packed_size(&msg);
    void* buf = malloc(len);
    
    if(buf == NULL){
        return -1;
    }
    message_t__pack(&msg, buf);

    if((write(rtable->sockfd,buf,len)) != len){
        return -1;
    }

    free(buf); 
    free(msg->data);
    
    //Waiting for server response

    len = MAX_BUF_SIZE; //whats the size of the response?
    buf = malloc(len);
    
    len = read(rtable->sockfd, buf, len);
    
    MessageT* resp = message_t__unpack(NULL, len, buf);
    if( resp==NULL || resp->opcode == MESSAGE_T__OPCODE__OP_ERROR ){
        return NULL;
    }

    free(buf);

    return resp; 
}

/* A função network_close() fecha a ligação estabelecida por
 * network_connect().
 */
int network_close(struct rtable_t * rtable) {
    // E mais isto
    return close(rtable->sockfd);
}
