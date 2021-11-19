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
