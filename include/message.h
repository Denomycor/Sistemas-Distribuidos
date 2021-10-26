#ifndef _NETWORK_H
#define _NETWORK_H

#define UNSIGNED_SIZE sizeof(char)

int read_all(int sockfd, char** data);

int write_all(int sockfd, char* data, int size);

#endif