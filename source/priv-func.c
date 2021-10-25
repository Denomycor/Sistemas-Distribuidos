#include "priv-func.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int strchars(const char* str){
    int i=0;
    while(str[i++]!='\0');
    return i;
}

int strmem(const char* str){
    return strchars(str)*sizeof(char);
}

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

char* sstrcpy(const char* str){
    size_t sz = strmem(str);
    char* new = malloc(sz);
    memcpy(new, str, sz);
    return new;
}

void malloc_error(){
    perror("malloc error\n");
    exit(-1);
}

void* throw_malloc(size_t size, void(*error)()){
    void* temp = malloc(size);
    if(temp == NULL){
        error();
    }
    return temp;
}
