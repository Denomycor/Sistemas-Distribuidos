/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "client/network_client.h"
#include "message/message.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

/* Esta função deve:
 * - Obter o endereço do servidor (struct sockaddr_in) a base da
 *   informação guardada na estrutura rtable;
 * - Estabelecer a ligação com o servidor;
 * - Guardar toda a informação necessária (e.g., descritor do socket)
 *   na estrutura rtable;
 * - Retornar 0 (OK) ou -1 (erro).
 */
int network_connect(struct rtable_t *rtable){
    rtable->socket.sin_family = AF_INET;
    rtable->socket.sin_port = htons(rtable->port);
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
MessageT *network_send_receive(struct rtable_t * rtable, MessageT *msg){
    int len = message_t__get_packed_size(msg);

    uint8_t* buf = malloc(len);
    if(buf == NULL){
        return NULL;
    }
    message_t__pack(msg, buf);

    if((write_all(rtable->sockfd,buf,len)) == -1){
        return NULL;
    }

    free(buf);

    if(msg->buffer.data!=NULL)
        free(msg->buffer.data);

    if((len = read_all(rtable->sockfd, &buf)) == -1){
        return NULL;
    }
    
    MessageT* resp = message_t__unpack(NULL, len, buf);

    free(buf);
    return resp;
}

/* A função network_close() fecha a ligação estabelecida por
 * network_connect().
 */
int network_close(struct rtable_t * rtable){
    return close(rtable->sockfd);
}
