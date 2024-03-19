#include <stdlib.h>
#include "menu.h"
#include "utils.h"

#define BufferLength 512

int main(){
    int nChoice;
    char *tableName = NULL;
    char *indexName = NULL;   

    do
    {
        nChoice = get_input();
        
        switch (nChoice){
        case 1:
            free(tableName);
            free(indexName);
            tableName = commandUse();
            if (!tableName) return -1;
            indexName = calloc(strlen(tableName) + 1, 1);
            if (!indexName) return -1;
            replaceExtensionByIdx(tableName, indexName);
            break;
        case 2:
            commandInsert(tableName, indexName);
            break;
        case 3:
            commandPrint(indexName);
            break;
        default:
            break;
    }
    } while (nChoice != 4);  
    free(tableName);
    free(indexName);
    return 0;
} 


int get_input() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(
        "1. Use\n"
        "2. Insert\n"
        "3. Print\n"
        "4. Exit.\n"
        );

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected = 0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 4)) {
            /*printf("%d", nSelected);*/
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

char* commandUse(){
    char *tableName = NULL;
    tableName = (char*)malloc(sizeof(char) * BufferLength);
    if(!tableName) return NULL;

    printf("Introduzca la tabla que quieres utilizar (archivo.dat):\n"); 
    if (fgets(tableName, BufferLength, stdin)){
        tableName[strcspn(tableName, "\n")]= '\0';
    } 
    printf("Nombre del .dat: %s\n", tableName);

    if (createTable(tableName) == false)
    {
        printf("Fallo en la ejecucion de createTable.\n");
        free(tableName);
        return NULL;
    }
    
    return tableName;  
}

int commandInsert(char *tableName, char *indexName){
    char *key;
    char *titulo;
    Book book;
    key = (char*)malloc(sizeof(char) * BufferLength);
    titulo = (char*)malloc(sizeof(char) * BufferLength);

    printf("Introduzca la clave a almacenar:\n");
    if(fgets(key, BufferLength, stdin)){
        key[strcspn(key, "\n")] = '\0';
    }
    printf("Clave: %s\n", key);

    printf("Introduzca el titulo a almacenar\n");
    if (fgets(titulo, BufferLength, stdin)){
        titulo[strcspn(titulo, "\n")] = '\0';
    }
    printf("Titulo: %s\n", titulo);

    strncpy(book.book_id, key, PK_SIZE);
    book.title_len = strlen(titulo);
    book.title = titulo;
    
    if (addTableEntry(&book, tableName, indexName) == false)
    {
        printf("Fallo en la ejecucion de addTableEntry.\n");
        free(key);
        free(titulo);
        return -1;
    }

    free(key);
    free(titulo);

    return 0;
}

void commandPrint(char *indexName){
    size_t level;
    char buffer[BufferLength];

    printf("Introduzca nivel del arbol a imprimir:\n");
    if(fgets(buffer, BufferLength, stdin)){
        buffer[strcspn(buffer, "\n")] = '\0';
    }
    level = (size_t) atoi(buffer);
    printf("Arbol: \n");  
    printTree(level, indexName);
}