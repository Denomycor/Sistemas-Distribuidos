/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "client/client_redundancy.h"

static int is_connected;
static zhandle_t* zh;
static char* watcher_ctx = "Client Primary Server Watcher";
static char* zhost_port = "127.0.0.1:2181";
static char* primary_path = "/kvstore/primary";
static int server_info_len = ZDATALEN;
static int is_watching = 0;
char* server_info_buf = NULL;

/**
* Watcher function for connection state change events
*/
void connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void* context) {   
    if (type == ZOO_SESSION_EVENT) {                                                          
        if (state == ZOO_CONNECTED_STATE) {                                               
            is_connected = 1;                                                         
        } else {                                                                          
            is_connected = 0;                                                         
        }                                                                                 
    }                                                                                         
}

/* Get the updated data and reset the watch
 */
static void data_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx) {
    if (state == ZOO_CONNECTED_STATE) {
        if (type == ZOO_CHANGED_EVENT) { 
            zoo_wget(wzh, primary_path, data_watcher,(void *)watcher_ctx, server_info_buf, &server_info_len, NULL);
        } 
    }
}

/* Starts the redundancy managers
 * Return 0 ok
 * Return <0 in case of error
 */
int init_redundancy_manager() {
    if ( is_connected ) {
        return 0;
    }
    zh = zookeeper_init(zhost_port, connection_watcher, 2000, 0, 0, 0);
    if (zh == NULL) {
        return -1;
    }
    return 0;
}

/* Start the watcher required by the client
 * Return 0 ok
 * Return <0 in case of error
 */
int start_watcher() {
    if ( !is_connected ) {
        return -1;
    }
    if ( is_watching ) {
        return 0;
    }
    server_info_buf = (char *)malloc(ZDATALEN * sizeof(char));
    if (server_info_buf == NULL) {
        return -1;
    }
    if (ZOK != zoo_wget(zh, primary_path, data_watcher, watcher_ctx, server_info_buf, &server_info_len, NULL)) {
        free(server_info_buf);
        return -1;
    }
    is_watching = 1;

    return 0;
}

/* Stops the redundancy managers
 * Return 0 ok
 * Return <0 in case of error
 */
void close_redundancy_manager() {
    if (!is_connected) {
        return;
    }
    zookeeper_close(zh);
    free(server_info_buf);
}