#ifndef NOMBRE_H
#define NOMBRE_H

#include <stdio.h>
#include <stdbool.h>

#define NO_DELETED_REGISTERS -1
#define INDEX_HEADER_SIZE 8
#define DATA_HEADER_SIZE 4
#define PK_SIZE 4

typedef struct book {
    char book_id [PK_SIZE]; /* primary key */
    size_t title_len; /* title length*/
    char * title ; /* string to be saved in the database */
} Book ;

/* Note that ingener a struct cannot be used to
handle tables in databases since the table structure
is unknown at compilation time */

typedef struct node {
    char book_id[PK_SIZE] ;
    int left, right, parent, offset;
}Node ;

/* Function prototypes .
see function descriptions in the following sections
All function return true if successor false if failed
except find Key which return true if register found
and false otherwise .
*/

bool createTable(const char * tableName);
bool createIndex(const char * indexName);
bool findKey ( const char * book_id , const char * indexName, int * nodeIDOrDataOffset ) ;
bool addTableEntry(Book * book, const char * tableName, const char * indexName);
bool addIndexEntry(char * book_id, int bookOffset, const char * indexName);
void printTree(size_t level, const char * indexName);
void replaceExtensionByIdx(const char *fileName, char * indexName);

#endif