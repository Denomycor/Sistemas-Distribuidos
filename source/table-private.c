#include "include\table-private.h"

int get_hash_index(const char* key ,int n){
    int counter = 0;
    for(int i=0; key[i]!='\0'; i++){
        counter += key[i];
    }
    return counter % n;
}