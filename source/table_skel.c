
#include "sdmessage.pb-c.h"
#include "table.h"
#include "entry.h"
#include "serialization.h"
#include "string.h"

extern struct table_t* g_table;

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
        msg->c_type == MESSAGE_T__C_TYPE__CT_RESULT;
        msg->data_size = 4;
        msg->data = malloc(sizeof(int));
        *msg->data = table_size(&g_table);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_DEL){
        if(table_del(&g_table, msg->data)==-1){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        }else{
            msg->opcode++;
        }
        msg->c_type == MESSAGE_T__C_TYPE__CT_NONE;
        free(msg->data);
        msg->data_size = 0;
        msg->data = NULL;

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_GET){
        struct data_t* temp;
        if(temp = table_get(&g_table, msg->data) == NULL){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
            msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            free(msg->data);
            msg->data = NULL;
            msg->data_size = 0;

        }else{
            msg->opcode++;
            msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
            free(msg->data);
            msg->data_size = data_to_buffer(temp, &msg->data);
        }

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_PUT){
        struct entry_t* temp = buffer_to_entry(msg->data, msg->data_size);
        if(table_put(&g_table, temp->key, temp->value)==-1){
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
            entry_destroy(temp);
        }else{
            msg->opcode++;
            free(temp);
        }
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        free(msg->data);
        msg->data = NULL;
        msg->data_size = 0;

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_GETKEYS){
        msg->opcode++;
        msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
        msg->data_size=0;

        char** k = table_get_keys(&g_table);
        int r=0;
        int buf_size=0;

        while(k[r]!=NULL){  
            msg->data_size++; //count nr of strings
            buf_size += strlen(k[r++])+1; //acc for chars in single buff size
        }

        buf_size = buf_size*sizeof(char); //multiply for sizeof chars
        char* buf = malloc(buf_size); //aloc single buf

        r=0;
        char* l = buf;
        while(k[r]!=NULL){
            memcpy(l, k[r], sizeof(char)*(strlen(k[r])+1)); ///copy for single buff
            l += sizeof(char)*(strlen(l)+1); //advance ptr to the start of next string
            r++;
        }

        msg->data = buf;

        table_free_keys(k);

    }else if(msg->opcode == MESSAGE_T__OPCODE__OP_PRINT){



    }else{
        return -1;
    }
    return 0;
}

