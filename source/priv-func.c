#include "priv-func.h"
#include <stdlib.h>
#include <string.h>

char* subsstr(const char* str, int st, int fn){
    size_t size = fn-st+1;
    char*const temp = malloc((size+1)*sizeof(char));
    memcpy(temp, str+st, size);
    temp[size]='\0';
    return temp;
}


void strapp(char** str, const char* app){
    *str = realloc(*str, (strlen(*str)+strlen(app)+1)*sizeof(char));
    memcpy(*str+strlen(*str), app, strlen(app)*sizeof(char));
}