/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "client/client_stub-private.h"
#include "client/client_stub.h"
#include "client/network_client.h"
#include "statistics/stats.h"
#include "server/server_redundancy.h"
#include "server/network_server.h"
#include "message/message.h"
#include "server/access_man.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

extern struct table_t* g_table;
extern struct statistics stats;
extern enum server_status g_status;
rw_mutex_t stats_mutex;

/* Receives the socket descriptor and handles a response from the server
 * to the client
 */
void* dispatch_thread(void* args){
    

    if(pthread_detach(pthread_self())!=0){
        printf("Error detaching the thread %li", pthread_self());
        return (void*)-1;
        free(args);
    }

    int sockfd = *(int*)args;
    free(args);

    MessageT* msg;
    while (1) {

        if((msg = network_receive(sockfd)) == NULL){
            printf("Error processing response at thread: %li - couldn't receive message", pthread_self());
            return (void*)-1;
        }

        if(msg->opcode == MESSAGE_T__OPCODE__OP_PUT || msg->opcode == MESSAGE_T__OPCODE__OP_DEL){
            char* backup = malloc(DATAMAXLEN);
            if(0 == server_zoo_get_backup(backup, DATAMAXLEN)){
                
                struct rtable_t* connection;
                connection = rtable_connect(backup);
                MessageT* msg2 = network_send_receive(connection, msg);
                
                if(msg2->opcode != MESSAGE_T__OPCODE__OP_ERROR){
                    if (invoke(msg) < 0){
                        printf("Error processing response at thread: %li couldn't resolve asnwer", pthread_self());
                        return (void*)-1;
                    }
                }else{
                    error_msg(msg);
                }
        
                rtable_disconnect(connection);
            }else{
                error_msg(msg);
            }

            free(backup);
        }else{
            if (invoke(msg) < 0){
                printf("Error processing response at thread: %li couldn't resolve asnwer", pthread_self());
                return (void*)-1;
            }
        }


        if(network_send(sockfd, msg) < 0){
            printf("Error processing response at thread: %li couldn't send message", pthread_self());
            return (void*)-1;
        }

    }

    return (void*)0;
}


/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port){
    
    signal(SIGPIPE, SIG_IGN);

    int sockfd;
    struct sockaddr_in server = {};
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        return -1;
    }

    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        close(sockfd);
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0){
        close(sockfd);
        return -1;
    };

    if (listen(sockfd, 0) < 0){
        return -1;
    };

    return sockfd;
}

/* Esta função deve:
 * - Aceitar uma conexão de um cliente;
 * - Receber uma mensagem usando a função network_receive;
 * - Entregar a mensagem de-serializada ao skeleton para ser processada;
 * - Esperar a resposta do skeleton;
 * - Enviar a resposta ao cliente usando a função network_send.
 */
int network_main_loop(int listening_socket){
    while(1) {
    
        int sockfd;
        if ((sockfd = accept(listening_socket,NULL,0)) < 0) {
            return -1;
        }

        int* args;
        if((args = malloc(sizeof(int))) == NULL){
            printf("Couldn't initialize a new thread to answer a request");
            continue;
        }
        *args = sockfd;

        pthread_t id;
        if(pthread_create(&id, NULL, dispatch_thread, args) != 0){
            printf("Couldn't initialize a new thread to answer a request");
            continue;
        }
    }
}

/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura MessageT.
 */
MessageT* network_receive(int client_socket){
    uint8_t* buf;
    int len;
    
    if((len = read_all(client_socket,&buf)) == -1){
        close(client_socket);
        return NULL;
    }
    MessageT* msg = message_t__unpack(NULL, len, buf);
    free(buf);
    return msg;
}

/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, MessageT *msg){
    int len = message_t__get_packed_size(msg);
    void* buf = malloc(len);
    
    
    if(buf == NULL) {
        close(client_socket);
        return -1;
    }
    message_t__pack(msg,buf);
    message_t__free_unpacked(msg,NULL);

    if(write_all(client_socket,buf,len) == -1){
        close(client_socket);
        return -1;
    }
    free(buf);
    return 0;
}

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close(int listening_socket){
    return close(listening_socket);
}

int prepare_socket(char* address_port){
    char* ip;
    short port;
    int sockfd;

    if(-1 == parse_address(address_port, &ip, &port)){
        printf("ERROR! - Incorrect address format");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1 || ip == NULL || port == -1){
        return -1;
    }

    struct sockaddr_in socket;

    socket.sin_family = AF_INET;
    socket.sin_port = htons(port);
    if(inet_pton(AF_INET, ip, &socket.sin_addr) < 1){
        close(sockfd);
        return -1;
    }

    if (connect(sockfd,(struct sockaddr *)&socket, sizeof(socket)) < 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void error_msg(MessageT* msg){
    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    if(msg->buffer.data!=NULL) free(msg->buffer.data);
    msg->buffer.data = NULL;
    msg->buffer.len = 0;
}