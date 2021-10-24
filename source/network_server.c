
#include "network_server.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

extern struct table_t* g_table;
extern int g_sockfd;

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port){
    int sockfd;
    struct sockaddr_in server;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

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
    if ((sockfd = accept(listening_socket,NULL,0)) < 0){ //nao tenho 100% certeza se os dois ultimos parametros estao corretos
        return -1;
    }
    MessageT* msg = network_receive(sockfd);
    if (invoke(msg) < 0){
        return -1;
    }
    if(network_send(sockfd, msg) < 0){
        return -1;
    }
    return 0;
}

/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura MessageT.
 */
MessageT* network_receive(int client_socket){
    int len = MAX_BUF_SIZE;
    void* buf = malloc(len);
    
    if(len = read(client_socket,buf,len)==-1){
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
        return -1;
    }
    message_t__pack(msg,buf);
    message_t__free_unpacked(msg,NULL);
    if(write(client_socket,buf,len) == -1){ //talvez != len nao seja a melhor verificacao
        return -1;
    }
    free(buf);
    //free client socket
    return 0;
}

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close(){
    
}
