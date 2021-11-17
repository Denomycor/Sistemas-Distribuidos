/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "network_server.h"
#include "message.h"
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

/* Receives the socket descriptor and handles a response from the server
 * to the client, returns 0 if no errors occured
 */
void* dispatch_thread(void* args){
    
    if(pthread_detach(gettip())!=0){
        printf("Error detaching the thread %i", gettip());
        return -1;
    }

    int sockfd = *(int*)args;
    free(args);

    MessageT* msg;
    if((msg = network_receive(sockfd)) == NULL){
        printf("Error processing response at thread: %i - couldnt receive message", gettid());
        return -1;
    }

    if (invoke(msg) < 0){
        printf("Error processing response at thread: %i couldnt resolve asnwer", gettid());
        return -1;
    }

    if(network_send(sockfd, msg) < 0){
        printf("Error processing response at thread: %i couldnt send message", gettid());
        return -1;
    }

    return 0;
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
    int sockfd;

    if (listen(listening_socket, 0) < 0){
        return -1;
    };
    if ((sockfd = accept(listening_socket,NULL,0)) < 0){ //the struct sockaddr wont be necessary
        return -1;
    }

    int* args = malloc(sizeof(int));
    if(args == NULL){
        return -2;
    }
    *args = sockfd;

    pthread_t id;
    if(pthread_create(&id, NULL, dispatch_thread, args)!=0){
        return -2;
    }

    return 0;
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
    close(client_socket);
    return 0;
}

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close(int listening_socket){
    return close(listening_socket);
}
