#ifndef MENUS_H
#define MENUS_H

#include "odbc.h"
typedef void (*print_fun)();

void navigate_main_menu(SQLHDBC dbc);

#endif