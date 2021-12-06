/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "helper/priv-func.h"
#include <stdlib.h>
#include <string.h>

void strapp(char** str, const char* app){
    *str = realloc(*str, (strlen(*str)+strlen(app)+1)*sizeof(char));
    memcpy(*str+strlen(*str), app, (strlen(app)+1)*sizeof(char));
}

int parse_address(const char* address_port, char** ip, int* port){
    size_t sz=-1;
    for(int i=0; i<strlen(address_port);i++){
        if(address_port[i] == ':'){
            *port = atoi(address_port+i+1);
            sz = sizeof(char)*(i+1);
            *ip = malloc(sz);
            memcpy(*ip, address_port, sizeof(char)*(i+1));
            (*ip)[i] = '\0';
        }
    }
    return sz;
}
