
#include "table.h"
#include "network_server.h"
#include "table_skel.h"
#include <stdlib.h>
#include <stdio.h>

struct table_t* g_table;
int g_sockfd;
    
//TODO
int main(int argc, char** argv) {
    
    int listening_socket;

    if (argc != 3) {
        perror("Must be called with 2 arguments -> [port] [n_of_lists]\n");
        exit(-1);
    }

    if (table_skel_init(atoi(argv[2]))) {
        perror("Couldn't create table program will exit");
        exit(-1);
    }
    short port = atoi(argv[1]);

    listening_socket = network_server_init(port);

    while(1) {
        if(network_main_loop(listening_socket)==-1){
            perror("Something went wrong");
            exit(-1);
        }
    }

    if(network_server_close(listening_socket)==-1){
        perror("Couldn't close listening socket");
        exit(-1);
    }
    table_skel_destroy();

}
