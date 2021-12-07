/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _PRIV_FUNC_H
#define _PRIV_FUNC_H

//appends a string to another
void strapp(char** str, const char* app);

int parse_address(const char* address_port, char** ip, short* port);

#endif
