/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "statistics/stats.h"
#include "helper/priv-func.h"
#include "dataStructs/table/table.h"
#include "server/network_server.h"
#include "server/table_skel.h"
#include "server/server_redundancy.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


struct table_t* g_table;
struct statistics stats; //Initialized at 0 because its a global variable
enum server_status g_status;
    
int main(int argc, char** argv) {
    
    int listening_socket;

    if (argc != 4) {
        printf("Must be called with 3 arguments -> [port] [n_of_lists] [zoo_ip:zoo_port]\n");
        exit(-1);
    }

    if (server_zoo_init(argv[3]) == -1){
        exit(-1);
    }

    char* address;
    myIp(&address);

    char* heap = malloc((strlen(address)+1)*sizeof(char));
    memcpy(heap, address, (strlen(address)+1)*sizeof(char));

    strapp(&heap, ":");
    strapp(&heap, argv[1]);

    do{
        g_status = server_zoo_register(heap, strlen(heap)+1);
        if(g_status == REPEAT){
            sleep(1);
        }
    }while(g_status == REPEAT);
    

    free(heap);
    server_zoo_setwatch(&g_status);

    if(g_status != NONE && g_status != ERROR){

        if (table_skel_init(atoi(argv[2]))) {
            perror("Couldn't create table program will exit");
            exit(-1);
        }
        short port = atoi(argv[1]);

        listening_socket = network_server_init(port);

        if(network_main_loop(listening_socket) < 0 ){
            printf("Error - Couldn't prepare server to listen");
        }

        if(network_server_close(listening_socket)==-1){
            table_skel_destroy();
            perror("Error - Couldn't close listening socket");
            exit(-1);
        }
        table_skel_destroy();

    }

    server_zoo_close();
}
