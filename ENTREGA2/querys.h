#ifndef QUERYS_H
#define QUERYS_H

#include "odbc.h"

void show_customer_balance(SQLHDBC dbc);
void show_customers_list(SQLHDBC dbc);
void show_customers_find(SQLHDBC dbc);

void show_orders_detail(SQLHDBC dbc);
void show_orders_open(SQLHDBC dbc);
void show_orders_range(SQLHDBC dbc);

void show_product_stock(SQLHDBC dbc);
void show_product_find(SQLHDBC dbc);

#endif
