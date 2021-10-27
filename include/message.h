#ifndef _NETWORK_H
#define _NETWORK_H

#include <stdint.h>

#define UNSIGNED_SIZE sizeof(uint32_t)

int read_all(int sockfd, uint8_t** data);

int write_all(int sockfd, char* data, int size);

#endif