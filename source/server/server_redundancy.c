/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "server/server_redundancy.h"
#include <string.h>
#include <unistd.h>


static u_int8_t is_connected;
static zhandle_t* zh;

void connection_watcher(zhandle_t *zh, int type, int state, const char *path, void* context){
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            is_connected = 1; 
        } else {
            is_connected = 0; 
        }
    } 
}

static void child_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx) {
    if (state == ZOO_CONNECTED_STATE){
        if (type == ZOO_CHILD_EVENT){

            enum server_status* this_server_status = watcher_ctx;

            //Move backup to primary
            if(*this_server_status == BACKUP && ZNONODE == zoo_exists(zh, "/kvstore/primary", 0, NULL)){
                
                void* buff = malloc(DATAMAXLEN);
                memset(buff, 0, DATAMAXLEN);

                if(ZOK != zoo_get(zh, "/kvstore/backup", 0, buff, DATAMAXLEN, NULL)){
                    printf("Error! - Couldn't get data at backup in zookeeper");
                }

                if(ZOK != zoo_create(zh, "/kvstore/primary", buff, DATAMAXLEN, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0)){
                    printf("Error! - Couldn't create primary node in zookeeper");
                }

                free(buff);

                if(ZOK != zoo_delete(zh, "/kvstore/backup", -1)){
                    printf("Error! - Couldn't delete backup node in zookeeper");
                }

                *this_server_status = PRIMARY_WITHOUT_BACKUP;
            }

            //Primary lost its backup
            if(*this_server_status == PRIMARY_WITH_BACKUP && ZNONODE == zoo_exists(zh, "/kvstore/backup", 0, NULL)){
                *this_server_status = PRIMARY_WITHOUT_BACKUP;
            }

            //Primary gained its backup
            if(*this_server_status == PRIMARY_WITHOUT_BACKUP && ZOK == zoo_exists(zh, "/kvstore/backup", 0, NULL)){
                *this_server_status = PRIMARY_WITH_BACKUP;
            }

            if (ZOK != zoo_wget_children(zh, "/kvstore", &child_watcher, watcher_ctx, NULL)) {
                printf("ERROR! - Couldn't set watch at /kvstore");
            }
        }
    }
}

int server_zoo_init(const char* zoo_host){
    if(is_connected) return 0;

    zh = zookeeper_init(zoo_host, connection_watcher, 2000, 0, 0, 0);
    if(zh == NULL){
        perror("Error! - Couldn't start zookeeper");
        return -1;
    }

    sleep(2);

    if(ZNONODE == zoo_exists(zh, "/kvstore", 0, NULL)){
        if(ZOK != zoo_create(zh, "/kvstore", NULL, -1, &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0)){
            perror("Error! - Couldn't start zookeeper");
            return -1;
        }
    }
    return 0;
}

enum server_status server_zoo_register(const char* data, size_t datasize){
    if(!is_connected) 
        return ERROR;

    struct String_vector children_list;
    u_int8_t has_primary=0, has_backup=0; 

    enum ZOO_ERRORS log = zoo_get_children(zh, "/kvstore",0 , &children_list);
    
    if(ZOK != log){
        printf("Error! - Couldn't get children in zookeeper");
        return ERROR;
    }
    for(int i=0; i<children_list.count; i++){
        if (0==strcmp(children_list.data[i], "backup")){
            has_backup=1;
        }
        if (0==strcmp(children_list.data[i], "primary")){
            has_primary=1;
        }
    }


    if(!has_primary && !has_backup){
        if(ZOK != zoo_create(zh, "/kvstore/primary", data, datasize, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0)){
            printf("Error! - Couldn't create primary node in zookeeper");
            return ERROR;
        }
        return PRIMARY_WITHOUT_BACKUP;
    }

    if(has_primary && !has_backup){
        log = zoo_create(zh, "/kvstore/backup", data, datasize, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
        if(ZOK != log){
            printf("Error! - Couldn't create backup node in zookeeper");
            return ERROR;
        }
        return BACKUP;
    }

    if(!has_primary && has_backup){
        return REPEAT;
    }

    return NONE; //has_primary && has_backup
}

int server_zoo_setwatch(enum server_status* status){
    if(!is_connected) 
        return -1;

    if(ZOK != zoo_wget_children(zh, "/kvstore", &child_watcher, status, NULL)){
        printf("ERROR! - Couldn't set watch at /kvstore");
        return -1;
    }
    return 0;
}

int server_zoo_get_primary(char* meta_data, size_t size){
    if(!is_connected) return -1;
    memset(meta_data, 0, size);
    if(ZOK != zoo_get(zh, "/kvstore/primary", 0, meta_data, size, NULL)){
        printf("Error! - Couldn't get data at primary in zookeeper");
        return -1;
    }
    return 0;
}

int server_zoo_get_backup(char* meta_data, size_t size){
    if(!is_connected) return -1;
    memset(meta_data, 0, size);
    if(ZOK != zoo_get(zh, "/kvstore/backup", 0, meta_data, size, NULL)){
        printf("Error! - Couldn't get data at backup in zookeeper");
        return -1;
    }
    return 0;
}

void server_zoo_close(){
    if(is_connected==1){
        zookeeper_close(zh);
        is_connected=0;
    }
}
