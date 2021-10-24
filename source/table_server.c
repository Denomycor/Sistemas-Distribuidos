
#include "table.h"
#include "network_server.h"
#include "table_skel.h"

struct table_t* g_table;
int g_sockfd;
    

int main(int argc, char** argv) {
    
    if (argc != 3) {
        printf("Deve chamar com dois argumentos no seguinte formato -> [port] [n_of_lists]\n");
        exit(0);
    }

    if (table_skel_init(atoi(argv[2]))) {
        printf("Couldn't create table program will exit");
        exit(0);
    }
    short port = atoi(argv[2]);

    network_server_init(port);

    while(1) {
        network_main_loop(port);
    }

    network_server_close();
    table_skel_destroy();

}