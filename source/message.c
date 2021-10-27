#include "message.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int read_all(int sockfd, uint8_t** data) {
    int alreadyRead = 0;
    int size;

    int* tmp = malloc(UNSIGNED_SIZE);

    read(sockfd, tmp, UNSIGNED_SIZE);

    size = ntohl(*tmp);
    *data = malloc(size);

    memcpy(*data, tmp, UNSIGNED_SIZE);
    alreadyRead += UNSIGNED_SIZE;

    free(tmp);

    while (alreadyRead != size) {
        int readNow = read(sockfd, *data+alreadyRead, UNSIGNED_SIZE);
        if ( readNow == -1 ) {
            return -1;
        }
        alreadyRead += readNow;
    }
    return alreadyRead;
}

int write_all(int sockfd, char* data, int size) {
    int sent = 0;

    while (sent != size) {
        int sentNow = write(sockfd, data, size);
        if ( sentNow == -1 ) {
            return -1;
        }
        sent += sentNow;
    }
    return sent;
}