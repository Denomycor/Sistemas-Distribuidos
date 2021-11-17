/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "client_stub.h"
#include "priv-func.h"
#include <stdio.h>
#include <string.h>

#define RESP_SIZE 512
#define RESP_SIZE_S "512"

int main(int argc, char** argv){

    struct rtable_t* table;

    if(argc != 2){
        printf("Error - must be called with <ip>:<port>\n");
        return -1;
    }

    struct {
        char com[RESP_SIZE];
        char* ops[3];
        int c;
    }parser;

    do {
        if((table = rtable_connect(argv[1])) == NULL){
            perror("Error - couldn't open socket");
            return -1;
        }

        parser.c = 0;
        memset(parser.ops, 0, 3*sizeof(char*));
        memset(parser.com, 0, RESP_SIZE);
        
        printf("\n-size\n-del<key>\n-get<key>\n-put<key><data>\n-getkeys\n-table_print\n-quit\n>>> ");
        fgets(parser.com,RESP_SIZE, stdin);
        parser.com[strlen(parser.com)-1] = '\0';
        printf("\n\n");
        

        char *reader = parser.com, *last = parser.com;


        while(*reader!='\0' && parser.c < 2 ){
            if(*reader == ' '){
                *reader = '\0';
                parser.ops[parser.c++] = last;
                last = reader+1;
            }
            reader++;
        }
        parser.ops[parser.c++] = last;

        if(strcmp(parser.ops[0],"size")==0){
            printf("The size of the table is: %i\n", rtable_size(table));

        }else if(strcmp(parser.ops[0], "del")==0){
            int res = rtable_del(table, strdup(parser.ops[1]));
            if(res!=-1){
                printf("The entry was deleted\n");
            }else{
                printf("Error deleting the entry\n");
            }

        }else if(strcmp(parser.ops[0], "get")==0){
            struct data_t* data = rtable_get(table, strdup(parser.ops[1]));
            if(data==NULL){
                printf("The entry wasn't found\n");
            }else{
                printf("The value is: ");
                for(int i=0; i<data->datasize; i++){
                    printf("%c", *((char*)data->data+i));
                }
                printf("\n");
            }
            data_destroy(data);

        }else if(strcmp(parser.ops[0], "put")==0){
            struct data_t d = {strlen(parser.ops[2]),parser.ops[2]};
            struct entry_t e = {parser.ops[1], &d};
            int res = rtable_put(table, &e);
            if(res!=-1){
                printf("The entry was added\n");
            }else{
                printf("Error adding the entry\n");
            }

        }else if(strcmp(parser.ops[0], "getkeys")==0){
            char** k = rtable_get_keys(table);
            for(int i=0; k[i]!=NULL;i++){
                printf("%s\n", k[i]);
            }
            rtable_free_keys(k);

        }else if(strcmp(parser.ops[0], "table_print")==0){
            rtable_print(table);

        }else if(strcmp(parser.ops[0], "quit") != 0){
            printf("%s", "Please insert a valid command\n");
        }

        rtable_disconnect(table);

    } while( strcmp(parser.ops[0], "quit") != 0 );

    return 0;
}