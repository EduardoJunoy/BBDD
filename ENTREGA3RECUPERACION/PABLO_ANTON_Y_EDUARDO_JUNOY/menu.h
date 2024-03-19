#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <string.h>

char *commandUse();

int commandInsert(char *tableName, char *indexName);

void commandPrint(char *indexName);

int get_input();

#endif