/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "dataStructs/list/list.h"

/* Struct that represents a hash table, has an array of pointers to each list
 * and the size of the array
 */
struct table_t {
    struct list_t** lists_ptr;
    int n;
};

/* Generates the hash code and applies the module operator to get the index 
 */
int get_hash_index(const char* key ,int n);

#endif
