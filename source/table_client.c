
#include "client_stub.h"
#include "network_client.h"
#include "client_stub-private.h"
#include <stdio.h>

/*TODO on client.exe
    -finish main func
    -verify if the rtable disconnect and close are done right
    -check for memory leaks and inconsistencies
    -op print

*/
int main(int argc, char** argv){

    struct rtable_t* table;

    if(table = rtable_connect(argv[1]) == NULL){
        perror("Error - couldn't open socket");
        return -1;
    }

    if(network_connect(table)==-1){
        perror("Error - couldn't connect to server");
        return -1;
    }

    do{
        printf("\n 0 - size\n 1 - del<key>\n 2 - get<key>\n 3 - put<key><data>\n 4 - getkeys\n 5 - table_print\n 6 - quit\n Please choose from 0 to 6 what you wish to do");
        
        //TODO: Read op and operands and call the corresponding message

    }while();

    if(network_close(table) == -1){
        perror("Error - couldn't close the socket");
        return -1;
    }

    return 0;
}