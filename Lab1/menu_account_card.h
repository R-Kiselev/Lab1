#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <format>
#include <sqlite3.h>
#include "Account.h"
#include "AccountCollection.h"
#include "BankCard.h"

enum class menu_options
{
    end = 0,
    
    accounts_info,
    add_account,
    delete_account,

    add_card,
    delete_card,
    
    set_name,
    set_balance,
    
    transfer_account,
    transfer_cards,

    test_assignment_operator,
    test_equality_operator
};

void menu_account_card(sqlite3* DB);

void display_menu();
void handle_accounts_info(const AccountCollection& ac);
void handle_add_account(AccountCollection& ac, sqlite3* DB);
void handle_delete_account(AccountCollection& ac, sqlite3* DB);
void handle_add_card(AccountCollection& ac, sqlite3* DB);
void handle_delete_card(AccountCollection& ac, sqlite3* DB);
void handle_set_name(AccountCollection& ac, sqlite3* DB);
void handle_set_balance(AccountCollection& ac, sqlite3* DB);
void handle_transfer_account(AccountCollection& ac, sqlite3* DB);
void handle_transfer_cards(const AccountCollection& ac, sqlite3* DB);
void handle_test_assignment_operator(const AccountCollection& ac, sqlite3* DB);
void handle_test_equality_operator(const AccountCollection& ac);