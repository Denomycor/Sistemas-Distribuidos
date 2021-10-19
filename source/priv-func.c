#include "priv-func.h"
#include <stdlib.h>

char* subsstr(const char* str, int st, int fn){
    size_t size = fn-st+1;
    char*const temp = malloc((size+1)*sizeof(char));
    memcpy(temp, str+st, size);
    temp[size]='\0';
    return temp;