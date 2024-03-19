#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "querys.h"

void main_menu_print(){
    printf(
        "1. Products.\n"
        "2. Orders.\n"
        "3. Customers.\n"
        "4. Exit.\n"
    );
}

void products_menu_print(){
    printf(
        "1. Stock.\n"
        "2. Find.\n"
        "3. Back.\n"
    );
} 

void orders_menu_print(){
    printf(
        "1. Open.\n"
        "2. Range.\n"
        "3. Detail.\n"
        "4. Back.\n"
    );
}

void customers_menu_print(){
    printf(
        "1. Find.\n"
        "2. List Products.\n"
        "3. Balance.\n"
        "4. Back.\n"
    );
}

int get_input(print_fun print_menu, int f, int l) {
    int nSelected = 0;
    char buf[16];

    do {

        print_menu();

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < f) || (nSelected > l)) {
            /*printf("%d", nSelected);*/
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < f) || (nSelected > l));

    return nSelected;
}

void navigate_products_menu(SQLHDBC dbc){
    int nChoice;
    do {
        nChoice = get_input(products_menu_print, 1, 3);
        switch (nChoice) {
            case 1: 
                show_product_stock(dbc);
                break;

            case 2:
                show_product_find(dbc);
                break;

            case 3: 
                printf("\n");
                break;
        }
    } while (nChoice != 3);
}

void navigate_orders_menu(SQLHDBC dbc){
    int nChoice;
    do {
        nChoice = get_input(orders_menu_print, 1, 4);
        switch (nChoice) {
            case 1: 
                show_orders_open(dbc);
                break;

            case 2:
                show_orders_range(dbc);
                break;

            case 3: 
                show_orders_detail(dbc);
                break;

            case 4:
                printf("\n");
                break;
        }
    } while (nChoice != 4);
}

void navigate_customers_menu(SQLHDBC dbc){
    int nChoice;
    do {
        nChoice = get_input(customers_menu_print, 1, 4);
        switch (nChoice) {
            case 1: 
                show_customers_find(dbc);
                break;

            case 2:
                show_customers_list(dbc);
                break;

            case 3:
                show_customer_balance(dbc);
                break;

            case 4:
                printf("\n");
                break;
        }
    } while (nChoice != 4);
}

void navigate_main_menu(SQLHDBC dbc){
    int nChoice;
    do {
        nChoice = get_input(main_menu_print, 1, 4);
        switch (nChoice) {
            case 1: 
                navigate_products_menu(dbc);
                break;

            case 2:
                navigate_orders_menu(dbc);
                break;

            case 3: 
                navigate_customers_menu(dbc);
                break;

            case 4:
                break;
        }
    } while (nChoice != 4);
}