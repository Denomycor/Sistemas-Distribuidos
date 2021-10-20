#include "client_stub.h"
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Deve chamar apenas com um argumento no seguinte formato -> [address]:[port]\n");
        exit(0);
    }

    struct rtable_t *tab = rtable_connect(argv[1]);

    char *exitCommand = "exit";

    while (1) {
        printf("introduza um comando\n");

        char *input = malloc(sizeof(char)*300);

        scanf(&input);
        
        // TODO translate text commands to client_stub function calls
        if (strcmp(input, exitCommand) == 0 ){
            break;
        }
    }

    rtable_disconnect(tab);
    
}