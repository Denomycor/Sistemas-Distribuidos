/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _SERVER_REDUNDANCY_H
#define _SERVER_REDUNDANCY_H

#include <zookeeper/zookeeper.h>


#define ZDATALEN 1024 * 1024
#define DATAMAXLEN 22 //255.255.255.255:65535\0

enum server_status{
    ERROR = -1, NONE, PRIMARY_WITH_BACKUP, PRIMARY_WITHOUT_BACKUP, BACKUP, REPEAT
};
/*
ERROR - This server yielded an error
NONE - This server connection will be refused by the three
PRIMARY_WITH_BACKUP - This server is primary and has a backup server, client request will be forwarded
PRIMARY_WITHOUT_BACKUP - This server is primary and doesn't have a backup server, client requests will be refused
BACKUP - This server is a backup
Repeat - This server will try to register again after sleeping x time
*/


int server_zoo_init(const char* zoo_host);

enum server_status server_zoo_register(const char* data, size_t datasize);

int server_zoo_setwatch(enum server_status* status);

int server_zoo_get_primary(char* meta_data, size_t size);

int server_zoo_get_backup(char* meta_data, size_t size);

void server_zoo_close();

#endif
