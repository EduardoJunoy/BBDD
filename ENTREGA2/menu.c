#include "menus.h"

int main(){
    SQLHENV env = (void*)0;
    SQLHDBC dbc = (void*)0;
    int ret;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return 1;
    }

    navigate_main_menu(dbc);

    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return 1;
    }

    return 0;
}