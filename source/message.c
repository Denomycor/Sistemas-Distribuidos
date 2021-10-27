#include "message.h"

int read_all(int sockfd, char** data) {
    int alreadyRead = 0;
    int size;

    //FIXME: qual e o tamanho do unsigned int??
    char *tmp = malloc(UNSIGNED_SIZE);

    read(sockfd, tmp, UNSIGNED_SIZE);

    //TODO: perceber como usar as funcoes htonl e ntohl
    size = 10;
    data = malloc(size);

    memcpy(data, tmp, UNSIGNED_SIZE);
    alreadyRead += UNSIGNED_SIZE;
    free(tmp);

    while (alreadyRead != size) {
        int readNow = read(sockfd, data+alreadyRead, UNSIGNED_SIZE);
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