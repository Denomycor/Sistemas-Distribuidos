/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "statistics/stats.h"
#include "dataStructs/table/table.h"
#include "server/network_server.h"
#include "server/table_skel.h"
#include <stdlib.h>
#include <stdio.h>

struct table_t* g_table;
struct statistics stats; //Initialized at 0 because its a global variable
    
int main(int argc, char** argv) {
    
    int listening_socket;

    if (argc != 3) {
        printf("Must be called with 2 arguments -> [port] [n_of_lists]\n");
        exit(-1);
    }

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
        perror("Error - Couldn't close listening socket");
        exit(-1);
    }
    table_skel_destroy();

}
