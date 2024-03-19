#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "querys.h"

#define BufferLength 512 /*the length of the input and output strings*/

void show_customer_balance(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */
    
    char ent[BufferLength] = "\0";
    char sal[BufferLength] = "\0";

    /*allocates memory for the handle*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    
    /*uses a prepared statement to prevent SQL injection*/
    ret = SQLPrepare(stmt,
    (SQLCHAR*)      "SELECT sum(payments.amount)-sum(quantityordered*priceeach) as balance FROM customers natural join orders natural join orderdetails natural join payments where customernumber = ?;"
                    , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }

    /*binds the parameter to the question mark from the prepared statement*/
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent, 0, NULL);

    /*recieves information from the user*/
    printf("Enter customer number > ");
    (void)fgets(ent, BufferLength, stdin);
    ent[strlen(ent) - 1] = '\0';
    
    /*executes the statement on the database*/
    (void)SQLExecute(stmt);
    
    /*binds the first column to a variable*/
    (void)SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) sal, BufferLength, NULL);

    /*prints the results to the terminal*/
    printf("+-----------------+\n");
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("|balance: %s|\n", sal);
    }
    printf("+-----------------+\n");
    (void)SQLCloseCursor(stmt);

    /*frees the memory allocated for the handles*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}

void show_customers_list(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */

    char ent[BufferLength] = "\0";
    char sal1[BufferLength] = "\0";
    char sal2[BufferLength] = "\0";

    /*allocates memory for the handle*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*uses a prepared statement for security reasons*/
    ret = SQLPrepare(stmt, 
    (SQLCHAR*) "SELECT productname, sum(quantityordered) FROM customers natural join orders natural join orderdetails natural join products WHERE customernumber = ? GROUP BY productname, productcode ORDER BY productcode;"
                , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }

    /*binds a parameter to the question mark in the prepared statements*/
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent, 0, NULL);

    /*asks for an input form the user*/
    printf("Enter customer number > ");
    (void)fgets(ent, BufferLength, stdin);
    ent[strlen(ent) - 1] = '\0';

    /*executes the query on the database*/
    (void)SQLExecute(stmt);

    /*binds variables to the column that corresponds to them*/
    (void)SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *)sal1, BufferLength, NULL);
    (void)SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *)sal2, BufferLength, NULL);
    
    /*prints the output to the terminal*/
    /*printf("+-----------------------------------------------------------------+\n");
    printf("|                Product Name                    |Quantity Ordered|\n");
    printf("+-----------------------------------------------------------------+\n");    */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s %s\n", sal1, sal2);
    }
    /*printf("+-----------------------------------------------------------------+\n");*/
    (void)SQLCloseCursor(stmt);

    /*frees the memory allocated to the handle*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}

void show_customers_find(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */
    
    char ent[BufferLength] = "\0";
    char code[BufferLength] = "\0";
    char name[BufferLength] = "\0";
    char firstname[BufferLength] = "\0";
    char lastname[BufferLength] = "\0";

    /*allocates memory for the handle*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    
    /*uses a prepared statement so that SQL injection isn't possible*/
    ret = SQLPrepare(stmt,
    (SQLCHAR*)      "SELECT customername, contactfirstname, contactlastname, customernumber FROM customers WHERE contactfirstname like ? OR contactlastname like ? ORDER BY customernumber;"
                    , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("prepare statements\n", stmt, SQL_HANDLE_ENV);
        return;
    }

    /*asks for user input and modifies it to fit the query*/
    printf("Enter customer name > ");
    (void)fgets(ent, BufferLength, stdin);
    ent[strlen(ent) - 1] = '\0';
    strcpy(name, "%%");
    strcat(name, ent);
    strcat(name, "%%");

    /*binds variables to the question marks on the prepared statement*/
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, name, 0, NULL);
    
    (void)SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, name, 0, NULL);

    /*executes the statement*/
    (void)SQLExecute(stmt);  
    
    /*Binds the columns to their designated variables*/
    (void)SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) name, BufferLength, NULL);
    (void)SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) firstname, BufferLength, NULL);
    (void)SQLBindCol(stmt, 3, SQL_C_CHAR,(SQLCHAR *) lastname, BufferLength, NULL);
    (void)SQLBindCol(stmt, 4, SQL_C_CHAR,(SQLCHAR *) code, BufferLength, NULL);

    /*print the output to the terminal*/
    /*printf("+-------------------------------------------------------------------------------------+\n");
    printf("|                         Customers with that characters                              |\n");
    printf("+-------------------------------------------------------------------------------------+\n");
    printf("|         Name          |                       Contact                      |  Code  |\n");
    printf("+-----------------------|-------------------------------------------------------------+\n");*/
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s %s %s %s\n", code, name, firstname, lastname);
    }    
    /*printf("+-----------------------+-------------------------------------------------------------+\n");*/
    (void)SQLCloseCursor(stmt);

    /*frees the memory allocated for the handle*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}



void show_orders_detail(SQLHDBC dbc){
    SQLHSTMT stmt1 = NULL, stmt2 = NULL, stmt3 = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */
    
    char ent[BufferLength] = "\0";
    char orderdate[BufferLength] = "\0";
    char status[BufferLength] = "\0";
    char price[BufferLength] = "\0";
    char code[BufferLength] = "\0";
    char quantity[BufferLength] = "\0";
    char priceEach[BufferLength] = "\0";

    /*allocates all the handles that will be needed*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt1);
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt3);

    /*ask user*/
    printf("Enter ordernumber > ");
    (void)fgets(ent, BufferLength, stdin);
    ent[strlen(ent) - 1] = '\0';

    /*prepares the statements for the three querys*/
    ret = SQLPrepare(stmt1,
    (SQLCHAR*)      "SELECT orderdate, status FROM orders WHERE ordernumber = ?;"
                    , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt1, SQL_HANDLE_ENV);
        return;
    }

    ret = SQLPrepare(stmt2,
    (SQLCHAR*)      "SELECT sum(quantityordered*priceeach) FROM orderdetails where ordernumber = ?;"
                    , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt2, SQL_HANDLE_ENV);
        return;
    }

    ret = SQLPrepare(stmt3,
    (SQLCHAR*)      "SELECT productcode, quantityordered, priceeach from orderdetails WHERE ordernumber = ? order by orderlinenumber;"
                    , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt3, SQL_HANDLE_ENV);
        return;
    }

    /*binding of parameters*/
    (void)SQLBindParameter(stmt1, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent, 0, NULL);
    (void)SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent, 0, NULL);
    (void)SQLBindParameter(stmt3, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent, 0, NULL);

    /*executes all the querys*/
    (void)SQLExecute(stmt1);
    (void)SQLExecute(stmt2);
    (void)SQLExecute(stmt3);

    /*binds variables to the columns*/
    (void)SQLBindCol(stmt1, 1, SQL_C_CHAR,(SQLCHAR *) orderdate, BufferLength, NULL);
    (void)SQLBindCol(stmt1, 2, SQL_C_CHAR,(SQLCHAR *) status, BufferLength, NULL);
    (void)SQLBindCol(stmt2, 1, SQL_C_CHAR,(SQLCHAR *) price, BufferLength, NULL);
    (void)SQLBindCol(stmt3, 1, SQL_C_CHAR,(SQLCHAR *) code, BufferLength, NULL);
    (void)SQLBindCol(stmt3, 2, SQL_C_CHAR,(SQLCHAR *) quantity, BufferLength, NULL);
    (void)SQLBindCol(stmt3, 3, SQL_C_CHAR,(SQLCHAR *) priceEach, BufferLength, NULL);

    /*prints the results from the querys*/
    
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt1))) {
        printf("Day ordered: %s  status: %s\n", orderdate, status);
    }
    (void)SQLCloseCursor(stmt1);

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt2))) {
        printf("Total price: %s\n", price);
    }
    (void)SQLCloseCursor(stmt2);

    /*printf("item code\tquantity ordered\tprice");*/
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt3))) {
        printf("%s %s %s\n", code, quantity, priceEach);
    }
    (void)SQLCloseCursor(stmt3);

    /*free the memory used by the handles*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt1, SQL_HANDLE_STMT);
        return;
    }

    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt2, SQL_HANDLE_STMT);
        return;
    }

    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt3);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt3, SQL_HANDLE_STMT);
        return;
    }
}

void show_orders_open(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */
    char sal[BufferLength] = "\0";

    /*allocates the handle that will be used for this query*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    /*this query does not have paramenters, so there is no need to use prepared statements*/
    (void)SQLExecDirect(stmt, (SQLCHAR*) "SELECT ordernumber FROM orders where shippeddate is NULL ORDER BY ordernumber;", SQL_NTS);
    /*we bind the first column of the output to sal*/
    (void)SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) sal, BufferLength, NULL);

    /*we then print the output to the terminal*/
    /*printf("+-----------+\n");
    printf("|Open Orders|\n");
    printf("+-----------+\n");*/
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\n", sal);
    }
    /*printf("+-----------+\n");*/
    (void)SQLCloseCursor(stmt);

    /*frees the memory allocated for the handle*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}

void show_orders_range(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */

    char ent[BufferLength] = "\0";
    char ent1[BufferLength] = "\0";
    char ent2[BufferLength] = "\0";
    char sal1[BufferLength] = "\0";
    char sal2[BufferLength] = "\0";
    char ordernumber[BufferLength] = "\0";
    SQLLEN ordershippedInd = SQL_NULL_DATA;
    /*allocates space in the memory for the handles*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    /*uses a prepared statement*/
    ret = SQLPrepare(stmt, 
    (SQLCHAR*) "SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate between ? AND ? ORDER BY ordernumber;"
                , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }
    
    /*requests user input*/
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void)fgets(ent, BufferLength, stdin);
    strncpy(ent2, ent+13, 10);
    strncpy(ent1, ent, 10);

    /*binds paramenters to variables*/
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent1, 0, NULL);
    (void)SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent2, 0, NULL);

    /*executes the query*/
    (void)SQLExecute(stmt);

    /*binds the columns to variables*/
    (void)SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *)ordernumber, BufferLength, NULL);
    (void)SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *)sal1, BufferLength, NULL);
    (void)SQLBindCol(stmt, 3, SQL_C_CHAR,(SQLCHAR *)sal2, BufferLength, &ordershippedInd);

    /*prints the output to the teerminal*/
    /*printf("+--------------------------------------------+\n");
    printf("|| Order Number|  Order Date  | Shipped Date ||\n");
    printf("+--------------------------------------------+\n");  */  
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s %s %s\n", ordernumber, sal1, ordershippedInd != SQL_NULL_DATA ? sal2 : "");
    }
    /*printf("+--------------------------------------------+\n");*/
    (void)SQLCloseCursor(stmt);

    /*frees the memory allocated for the handle*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}



void show_product_stock(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */

    char ent[BufferLength] = "\0";
    char sal[BufferLength] = "\0";

    /*allocates memory for the handle*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /*uses a prepared statement*/
    ret = SQLPrepare(stmt, 
    (SQLCHAR*) "select quantityinstock from products where productcode = ?;"
                , SQL_NTS);
   
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }
    
    /*asks for user input*/
    printf("Enter productcode > ");
    (void)fgets(ent, BufferLength, stdin);
    ent[strlen(ent) - 1] = '\0';

    /*binds the query parameters to variables*/
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, ent, 0, NULL);
    /*executes the statement*/
    (void)SQLExecute(stmt);

    /*binds the column to a variable*/
    (void)SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *)sal, BufferLength, NULL);

    /*prints the output to the terminal*/
    /*printf("+---------+\n");
    printf("|  Stock  |\n");
    printf("+---------+\n");*/  
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\n", sal);
    }
    /*printf("+---------+\n");*/
    (void)SQLCloseCursor(stmt);

    /*frees the handle*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}

void show_product_find(SQLHDBC dbc){
    SQLHSTMT stmt = NULL;
    SQLRETURN ret2; /* ODBC API return status */
    int ret; /* odbc.c */
    
    char ent[BufferLength] = "\0";
    char code[BufferLength] = "\0";
    char name[BufferLength] = "\0";
    
    /*allocates memory for the handle*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    
    /*uses a preparde statement*/
    ret = SQLPrepare(stmt,
    (SQLCHAR*)      "SELECT productcode, productname from products where  productname like ? order by productcode;"
                    , SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("prepare statements\n", stmt, SQL_HANDLE_ENV);
        return;
    }

    /*requests user input*/
    printf("Enter productname > ");
    (void)fgets(ent, BufferLength, stdin);
    ent[strlen(ent) - 1] = '\0';
    strcpy(name, "%%");
    strcat(name, ent);
    strcat(name, "%%");

    /*binds the parameters to variables*/
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_C_CHAR, 
                                0, 0, name, 0, NULL);

    /*executes the query*/
    (void)SQLExecute(stmt);  
    
    /*binds the columns to variables*/
    ret2 = SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) code, BufferLength, NULL);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("bind first column", stmt, SQL_HANDLE_STMT);
        return;
    }
    /*ret2 = SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) name, BufferLength, NULL);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("bind second column", stmt, SQL_HANDLE_STMT);
        return;
    }*/
    /*prints the output to the termial*/
    /*printf("+---------------------------------------+\n");
    printf("|        Products with that name        |\n");
    printf("+---------------------------------------+\n");
    printf("|   Code   |            Name            |\n");
    printf("+----------+----------------------------+\n");*/

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\n", code);   /*printf("%s %s\n", code, name);*/
    }    
    /*printf("+----------+----------------------------+\n");*/
    (void)SQLCloseCursor(stmt);

    /*frees the memory allocated to the handle*/
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
}