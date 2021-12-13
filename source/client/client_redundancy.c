/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "client/client_redundancy.h"

static int connected;
static zhandle_t* zh;
static char* watcher_ctx = "Client Primary Server Watcher";
static char* zhost_port = "127.0.0.1:2181";
static char* primary_path = "/kvstore/primary";
static int is_watching = 0;
static int new = 0;
char* server_info_buf = NULL;

/**
* Watcher function for connection state change events
*/
void connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void* context) {   
    if (type == ZOO_SESSION_EVENT) {                                                          
        if (state == ZOO_CONNECTED_STATE) {                                               
            connected = 1;                                                         
        } else {                                                                          
            connected = 0;                                                         
        }                                                                                 
    }                                                                                         
}

/* Get the updated data and reset the watch
 */
void data_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx) {
    if (state == ZOO_CONNECTED_STATE) {
        if (type == ZOO_CHILD_EVENT) {

            if(ZOK == zoo_exists(zh, "/kvstore/primary", 0, NULL)){
                int server_info_len = ZDATALEN;
                if(ZOK != zoo_get(zh, primary_path, 0, server_info_buf,&server_info_len, NULL)){
                    printf("ERROR! - Couldn't get server address");
                }
            }
            new = 1;
        } 
    }

    if (ZOK != zoo_wget_children(zh, "/kvstore", &data_watcher, watcher_ctx, NULL)) {
        printf("ERROR! - Couldn't set watch at /kvstore");
    }       
}

/* Returns the state of the connection
 * Return 1 connected
 * Return 0 not connected
 */
int is_connected() {
    return connected;
}

/* Starts the redundancy managers
 * Return 0 ok
 * Return <0 in case of error
 */
int init_redundancy_manager() {
    if ( connected ) {
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
    if ( !connected ) {
        return -1;
    }
    if ( is_watching ) {
        return 0;
    }
    server_info_buf = (char *)malloc(ZDATALEN * sizeof(char));
    if (server_info_buf == NULL) {
        return -1;
    }
    int server_info_len = ZDATALEN;
    if (ZOK != zoo_get(zh, primary_path, 0, server_info_buf,&server_info_len, NULL)) {
        free(server_info_buf);
        return -1;
    }

    if (ZOK != zoo_wget_children(zh, "/kvstore", &data_watcher, watcher_ctx, NULL)) {
        printf("ERROR! - Couldn't set watch at /kvstore");
        return -1;
    }

    new = 1;
    is_watching = 1;

    return 0;
}

/* Returns 1 if the function has never been called after an update to the watched value 
 * Return 0 otherwise
 */
int is_new() {
    if (new) {
        new = 0;
        return 1;
    }
    return 0;
}

/* Stops the redundancy managers
 * Return 0 ok
 * Return <0 in case of error
 */
void close_redundancy_manager() {
    if (!connected) {
        return;
    }
    zookeeper_close(zh);
    free(server_info_buf);
    connected = 0;
    is_watching = 0;
}