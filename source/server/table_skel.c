/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "statistics/stats.h"
#include "server/table_skel.h"
#include "message/serialization.h"
#include "server/access_man.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern struct table_t* g_table;
extern struct statistics stats;
extern rw_mutex_t stats_mutex;
rw_mutex_t table_mutex;

/* Inicia o skeleton da tabela.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). O parâmetro n_lists define o número de listas a
 * serem usadas pela tabela mantida no servidor.
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int table_skel_init(int n_lists){
    if(rw_mutex_init(&table_mutex)!=0){
        return -1;
    }
    if(rw_mutex_init(&stats_mutex)!=0){
        return -1;
    }

    return (g_table = table_create(n_lists)) == NULL ? -1:0;  
}

/* Liberta toda a memória e recursos alocados pela função table_skel_init.
 */
void table_skel_destroy(){
    rw_mutex_destroy(&table_mutex);
    rw_mutex_destroy(&stats_mutex);
    table_destroy(g_table);
}

/* Executa uma operação na tabela (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura MessageT para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, tabela nao incializada)
*/
int invoke(MessageT *msg){

    struct timeval clock;
    start_timing(&clock);
    int op_code = msg->opcode;


    if(msg->opcode == MESSAGE_T__OPCODE__OP_SIZE){
        read_exclusive_lock(&table_mutex);
        msg->opcode++;
        msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
        msg->buffer.len = 4;
        msg->buffer.data = malloc(sizeof(int));
        *msg->buffer.data = table_size(g_table);
        read_exclusive_unlock(&table_mutex);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_DEL){
        write_exclusive_lock(&table_mutex);
        if(table_del(g_table, msg->buffer.data)==-1){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        }else{
            msg->opcode++;
        }
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        free(msg->buffer.data);
        msg->buffer.len = 0;
        msg->buffer.data = NULL;
        write_exclusive_unlock(&table_mutex);
    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_GET){
        read_exclusive_lock(&table_mutex);
        struct data_t* temp;
        if((temp = table_get(g_table, msg->buffer.data)) == NULL){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
            msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            free(msg->buffer.data);
            msg->buffer.data = NULL;
            msg->buffer.len = 0;

        }else{
            msg->opcode++;
            msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
            free(msg->buffer.data);
            msg->buffer.len = data_to_buffer(temp, (char**)&msg->buffer.data);
        }
        read_exclusive_unlock(&table_mutex);
        data_destroy(temp);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_PUT){
        write_exclusive_lock(&table_mutex);
        struct entry_t* temp = buffer_to_entry(msg->buffer.data, msg->buffer.len);
        if(table_put(g_table, temp->key, temp->value)==-1){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        }else{
            msg->opcode++;
        }
        entry_destroy(temp);
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        free(msg->buffer.data);
        msg->buffer.data = NULL;
        msg->buffer.len = 0;
        write_exclusive_unlock(&table_mutex);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_GETKEYS){
        read_exclusive_lock(&table_mutex);
        msg->opcode++;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
        msg->buffer.len=0;

        char** k = table_get_keys(g_table);
        int r=0;
        msg->buffer.len=0;

        while(k[r]!=NULL){
            msg->buffer.len += strlen(k[r++])+1; //acc for chars in single buff size
        }

        msg->buffer.len = msg->buffer.len*sizeof(char); //multiply for sizeof chars
        char* buf = malloc(msg->buffer.len); //aloc single buf

        r=0;
        char* l = buf;
        while(k[r]!=NULL){
            memcpy(l, k[r], sizeof(char)*(strlen(k[r])+1)); ///copy for single buff
            l += sizeof(char)*(strlen(l)+1); //advance ptr to the start of next string
            r++;
        }

        msg->buffer.data = buf;

        table_free_keys(k);
        read_exclusive_unlock(&table_mutex);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_PRINT){
        read_exclusive_lock(&table_mutex);
        msg->opcode++;
        msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
        msg->buffer.data = table_to_string(g_table);
        msg->buffer.len = strlen(msg->buffer.data)+1;
        read_exclusive_unlock(&table_mutex);


    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_STATS){
        msg->buffer.data = malloc(sizeof(struct statistics));
        if(msg->buffer.data == NULL){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
            msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            msg->buffer.len = 0;
        }else{
            msg->buffer.len = sizeof(struct statistics);
            if(read_exclusive_lock(&stats_mutex)!=0){
                printf("Error processing response at thread: %li couldn't lock read_exclusive at invoke", pthread_self());
                msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                msg->buffer.len = 0;
            }else{
                memcpy(msg->buffer.data, &stats, msg->buffer.len);
                if(read_exclusive_unlock(&stats_mutex)!=0){
                    printf("Error processing response at thread: %li couldn't unlock read_exclusive at invoke", pthread_self());
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                    msg->buffer.len = 0;
                }else{

                    msg->opcode++;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
                }
            }
        }

    }else{
        return -1;
    }

    if(!(op_code > 60 || op_code < 10)){
        double ms = stop_timing(&clock);

        if(write_exclusive_lock(&stats_mutex)!=0){
            printf("Error processing response at thread: %li couldn't lock write_exclusive", pthread_self());
            return -1;
        }

        update_stats(&stats, op_code, ms);
        
        if(write_exclusive_unlock(&stats_mutex)!=0){
            printf("Error processing response at thread: %li couldn't unlock write_exclusive", pthread_self());
            return -1;
        }
    }

        return 0;
}

