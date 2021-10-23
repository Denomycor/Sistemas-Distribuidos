#ifndef _PRIV_FUNC_H
#define _PRIV_FUNC_H

//Gets the number of chars in a string, including the \0 char (==strlen()+1)
int strchars(const char* str);

//Gets the number of bytes of a string (==(strlen()+1)*sizeof(char))
int strmem(const char* str);

//returns a pointer to a new substring
char* subsstr(const char* str, int st, int fn);

//appends a string to another
void strapp(char** str, const char* app);

//returns a pointer to a copy of a string
char* sstrcpy(const char* str)

#endif