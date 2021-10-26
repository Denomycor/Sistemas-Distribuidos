
#include "client_stub.h"
#include "network_client.h"
#include "client_stub-private.h"
#include "priv-func.h"
#include <stdio.h>
#include <string.h>

#define RESP_SIZE 512

/*TODO on client.exe
    -finish main func
    -verify if the rtable disconnect and close are done right
    -check for memory leaks and inconsistencies
    -op print

*/
int main(int argc, char** argv){

    struct rtable_t* table;

    if(argc != 2){
        perror("Error - must be called with <ip>:<port>\n");
        return -1;
    }


    if((table = rtable_connect(argv[1])) == NULL){
        perror("Error - couldn't open socket");
        return -1;
    }

    if(network_connect(table)==-1){
        perror("Error - couldn't connect to server");
        return -1;
    }

        struct {
            char com[RESP_SIZE];
            char* ops[3];
            int c;
        }parser = {};
    do{

        

        printf("\n 0 - size\n 1 - del<key>\n 2 - get<key>\n 3 - put<key><data>\n 4 - getkeys\n 5 - table_print\n 6 - quit\n Please choose from 0 to 6 what you wish to do: ");
        scanf("%s", parser.com);
        

        char *reader = parser.com, *last = parser.com;
        while(*reader!='\0'){
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
            int res = rtable_del(table, sstrcpy(parser.ops[1]));
            if(res!=-1){
                printf("The entry was deleted\n");
            }else{
                printf("Error deleting the entry\n");
            }

        }else if(strcmp(parser.ops[0], "get")==0){
            struct data_t* data = rtable_get(table, sstrcpy(parser.ops[1]));
            if(data==NULL){
                printf("The entry wasn't found\n");
            }else{
                printf("The value is %.*x\n", data->datasize, data->data);
            }
            data_destroy(data);

        }else if(strcmp(parser.ops[0], "put")==0){
            struct data_t d = {strlen(parser.ops[2]),parser.ops+2};
            struct entry_t e = {parser.ops[1], &d};
            int res = rtable_put(table, &e);
            if(res!=-1){
                printf("The entry was added\n");
            }else{
                printf("Error adding the entry\n");
            }

        }else if(strcmp(parser.ops[0], "getKeys")==0){
            char** k = rtable_get_keys(table);
            for(int i=0; k[i]!=NULL;i++){
                printf("%s\n", k[i]);
            }
            rtable_free_keys(k);

        }else if(strcmp(parser.ops[0], "table_print")==0){
            rtable_print(table);

        }else if(!strcmp(parser.ops[0], "quit")==0){
            printf("%s", "Please insert a valid command\n");
        }

    }while(!strcmp(parser.ops[0], "quit")==0);

    if(network_close(table) == -1){
        perror("Error - couldn't close the socket");
        return -1;
    }

    return 0;
}