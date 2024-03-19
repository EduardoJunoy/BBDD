#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* hay que crear un libro de prueba pidiendole los datos al usuario para probarlo todo*/

void _printTreeRec(size_t level, size_t depth, int node_act_id, FILE* index);
Node read_node(FILE *index, int node_id);
void writeData(Book * book, FILE * table, int Offset);
void write_node(FILE *index, Node* n, int node_id);

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    size_t l = 0;
    if(!fileName || !indexName) return;
    l = strlen(fileName);
    strncat(indexName, fileName, l - 3);
    strcat(indexName, "idx");
}

bool createTable(const char * tableName){
    FILE *f = NULL;
    int deleted = NO_DELETED_REGISTERS;
    char * indexName = NULL;
    bool out;

    if(!tableName) return NULL;

    f = fopen(tableName, "rb");
    if(!f){
        f = fopen(tableName, "wb+");
        fwrite(&deleted, sizeof(int), 1, f);
    }
    if (!f)
    {
        return false;
    }
    fclose(f);

    indexName = calloc(strlen(tableName) + 1, 1);
    replaceExtensionByIdx(tableName, indexName);
    out = createIndex(indexName);
    free(indexName);
    return out;
}

bool createIndex(const char * indexName){
    FILE *f = NULL;
    int deleted = -1;
    if(!indexName) return false;

    f = fopen(indexName, "rb");
    if(!f){
        f = fopen(indexName, "wb+");
        fwrite(&deleted, sizeof(int), 1, f);
        fwrite(&deleted, sizeof(int), 1, f);
    }
    if (!f)
    {
        return false;
    }
    fclose(f);
    return true;
}

bool findKey ( const char * book_id , const char * indexName, int * nodeIDOrDataOffset ){
    Node n;
    FILE *index = fopen(indexName, "rb");
    int cmp;
    int root;
    if(!book_id || !indexName || !nodeIDOrDataOffset) return false;


    fseek(index, 0, SEEK_SET);
    fread(&root, sizeof(int), 1, index);
    if(root == -1) {
        (*nodeIDOrDataOffset) = DATA_HEADER_SIZE;
        return false;
    }

    n = read_node(index, root);
    (*nodeIDOrDataOffset) = root;
    while(true){
        cmp = memcmp(book_id, n.book_id, PK_SIZE);
        if(cmp > 0){
            if(n.right == -1){
                fclose(index);
                return false;
            }
            (*nodeIDOrDataOffset) = n.right;
            n = read_node(index, n.right);
        }
        else if(cmp < 0){
            if (n.left == -1)
            {
                fclose(index);
                return false;
            }
            (*nodeIDOrDataOffset) = n.left;
            n = read_node(index, n.left);
        }
        else{
            (*nodeIDOrDataOffset) = n.offset;
            fclose(index);
            return true;
        }
    }
}

bool addTableEntry(Book * book, const char * tableName, const char * indexName){
    FILE *table = NULL;
    int nodeIdOrDataOffset = 0;
    int bookOffset = 0, pointer = -1, prev = 0;
    size_t reg_size = 0;
    bool found = false;
    
    if(!tableName || !book || !indexName) return false;

    if(findKey(book->book_id, indexName, &nodeIdOrDataOffset)) return false;
    table = fopen(tableName, "rb+");
    if(!table) return false;

    /*reads the pointer to the deleted list*/
    fread(&pointer, sizeof(int), 1, table);
    while(pointer != NO_DELETED_REGISTERS && !found){
        prev = bookOffset;
        bookOffset = pointer;
        fseek(table, pointer, SEEK_SET);
        fread(&pointer, sizeof(int), 1, table);
        fread(&reg_size, sizeof(int), 1, table);
        found = reg_size < book->title_len;
    }

    if(!found){
        /*finds the offset of the end of the file, 
        if there are no deleted registers uses it as the offset*/
        fseek(table, 0, SEEK_END);
        bookOffset = ftell(table);
    }
    else{
        fseek(table, prev, SEEK_SET);
        fwrite(&pointer, sizeof(int), 1, table);
    }
    
    writeData(book, table, bookOffset);
    fclose(table);
    
    return addIndexEntry(book->book_id, bookOffset, indexName);
}

void writeData(Book * book, FILE * table, int Offset){
    fseek(table, Offset, SEEK_SET);

    /*writes the book information to the table file*/
    fwrite(book->book_id, sizeof(char), PK_SIZE, table);
    fwrite(&book->title_len, sizeof(int), 1, table);
    fwrite(book->title, sizeof(char), book->title_len, table);
}

bool addIndexEntry(char * book_id, int bookOffset, const char * indexName){
    FILE *index = NULL;
    bool aux;
    int id, new_id;
    Node n, aux_n;
    int root = 0;
    int deleted = -2;

    if (!book_id || bookOffset < 0  || !indexName) return false;

    aux = findKey(book_id, indexName, &id);
    if(aux) return false;

    /*prepares the node to be inserted*/
    strncpy(n.book_id, book_id, PK_SIZE);
    n.offset = bookOffset;
    n.right = -1;
    n.left = -1;
    n.parent = id;

    index = fopen(indexName, "rb+");
    if(!index) return false;

    /*reads deleted and root*/
    fread(&root, sizeof(int), 1, index);
    fread(&deleted, sizeof(int), 1, index);

    /*if there is no nodes it adds one and changes the header*/
    if(root == -1){
        root = 0;
        fseek(index, 0, SEEK_SET);
        fwrite(&root, sizeof(int), 1, index);

        /*write the data to the file*/
        fseek(index, 0, SEEK_END);
        fwrite(&n, sizeof(Node), 1, index);
        fclose(index);
        return true;
    }

    /*creates a new id that would be at the end of the file*/
    fseek(index, 0, SEEK_END);
    new_id = (ftell(index) - INDEX_HEADER_SIZE)/sizeof(Node);

    if(deleted >= 0){
        aux_n = read_node(index, deleted);
        /*update header*/
        fseek(index, sizeof(int), SEEK_SET);
        fwrite(&aux_n.left, sizeof(int), 1, index);

        /*we will use the id of the deleted node*/
        new_id = deleted;
    }

    /*update the parent*/
    aux_n = read_node(index, n.parent);
    if(strncmp(book_id, aux_n.book_id, PK_SIZE)>0){
        aux_n.right = new_id;
    }
    else{
        aux_n.left = new_id;
    }
    write_node(index, &aux_n, n.parent);
    
    /*here we have to change the father node and add the new node at the end of the file*/
    write_node(index, &n, new_id);

    fclose(index);
    return true;
}

void printTree(size_t level, const char * indexName){
    FILE *index = NULL;
    Node n;
    int root;
    char aux[PK_SIZE + 1];

    index = fopen(indexName, "rb+");
    if (!index)
    {
        return ;
    }
    fseek(index, 0, SEEK_SET);
    fread(&root, sizeof(int), 1, index);
    if(root == -1) return;

    n = read_node(index, root);
    strncpy(aux, n.book_id, PK_SIZE);
    aux[PK_SIZE] = '\0';
    printf("%s (%d): %d\n", aux, root, n.offset);

    if(n.left != -1 && level >= 1){
        printf("\tl ");
        _printTreeRec(level, 1, n.left, index);
    }

    if(n.right != -1 && level >= 1){
        printf("\tr ");
        _printTreeRec(level, 1, n.right, index);
    }
    fclose(index);
}

void _printTreeRec(size_t level, size_t depth, int node_act_id, FILE* index){
    Node n;
    size_t i;
    char aux[PK_SIZE + 1];
    n = read_node(index, node_act_id);

    strncpy(aux, n.book_id, PK_SIZE);
    aux[PK_SIZE] = '\0';
    printf("%s (%d): %d\n", aux, node_act_id, n.offset);

    depth++;
    if(n.left != -1 && level >= depth){
        for(i = 0; i<depth; ++i) printf("\t");
        printf("l ");
        _printTreeRec(level, depth, n.left, index);
    }

    if(n.right != -1 && level >= depth){
        for(i = 0; i<depth; ++i) printf("\t");
        printf("r ");
        _printTreeRec(level, depth, n.right, index);
    }
}

Node read_node(FILE *index, int node_id){
    Node n;
    fseek(index, INDEX_HEADER_SIZE + node_id*sizeof(Node), SEEK_SET);
    fread(&n, sizeof(Node), 1, index);

    return n;
}

void write_node(FILE *index, Node* n, int node_id){
    fseek(index, node_id*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET);
    fwrite(n, sizeof(Node), 1, index);
}