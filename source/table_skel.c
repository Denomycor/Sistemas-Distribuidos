/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "table_skel.h"
#include "serialization.h"
#include <string.h>
#include <stdlib.h>
#include "stats.h"

extern struct table_t* g_table;
extern stats_t stats;

/* Inicia o skeleton da tabela.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). O parâmetro n_lists define o número de listas a
 * serem usadas pela tabela mantida no servidor.
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int table_skel_init(int n_lists){
    return (g_table = table_create(n_lists)) == NULL ? -1:0;  
}

/* Liberta toda a memória e recursos alocados pela função table_skel_init.
 */
void table_skel_destroy(){
    table_destroy(g_table);
}

/* Executa uma operação na tabela (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura MessageT para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, tabela nao incializada)
*/
int invoke(MessageT *msg){


    if(msg->opcode == MESSAGE_T__OPCODE__OP_SIZE){
        msg->opcode++;
        msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
        msg->buffer.len = 4;
        msg->buffer.data = malloc(sizeof(int));
        *msg->buffer.data = table_size(g_table);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_DEL){
        if(table_del(g_table, msg->buffer.data)==-1){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        }else{
            msg->opcode++;
        }
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        free(msg->buffer.data);
        msg->buffer.len = 0;
        msg->buffer.data = NULL;

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_GET){
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
            msg->buffer.len = data_to_buffer(temp, &msg->buffer.data);
        }
        data_destroy(temp);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_PUT){
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

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_GETKEYS){
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

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_PRINT){
        msg->opcode++;
        msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
        msg->buffer.data = table_to_string(g_table);
        msg->buffer.len = strlen(msg->buffer.data)+1;


    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_STATS){
        msg->buffer.data = malloc(sizeof(stats_t));
        if(msg->buffer.data == NULL){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
            msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            msg->buffer.len = 0;
        }else{
            msg->buffer.len = sizeof(stats_t);
            memcpy(msg->buffer.data, &stats, msg->buffer.len);
            msg->opcode++;
            msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
        }

    }else{
        return -1;
    }
    return 0;
}

