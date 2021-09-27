/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "table-private.h"

int get_hash_index(const char* key ,int n){
    int counter = 0;
    for(int i=0; key[i]!='\0'; i++){
        counter += key[i];
    }
    return counter % n;
}