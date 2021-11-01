/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _PRIV_FUNC_H
#define _PRIV_FUNC_H

//Gets the number of chars in a string, including the \0 char (==strlen()+1)
int strchars(const char* str);

//Gets the number of bytes of a string (==(strlen()+1)*sizeof(char))
int strmem(const char* str);

//appends a string to another
void strapp(char** str, const char* app);

#endif
