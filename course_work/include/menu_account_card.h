#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <format>
#include "../sqlite/sqlite3.h"
#include "../include/Bank/BankService.h"
#include "../include/SocialStatus/SocialStatusService.h"
#include "../include/Client/ClientService.h"
#include "../include/Account/AccountService.h"
#include "../include/Card/CardService.h"

enum class menu_options
{
    end = 0,
    
    accounts_info,
    add,
    delete_account,

    add,
    delete_card,
    
    set_name,
    set_balance,
    
    transfer_account,
    transfer_cards,

    test_assignment_operator,
    test_equality_operator
};

void menu_account_card(sqlite3* DB);

//void display_menu();
//void handle_accounts_info(const AccountCollection& ac);
//void handle_add(AccountCollection& ac, sqlite3* DB);
//void handle_delete_account(AccountCollection& ac, sqlite3* DB);
//void handle_add(AccountCollection& ac, sqlite3* DB);
//void handle_delete_card(AccountCollection& ac, sqlite3* DB);
//void handle_set_name(AccountCollection& ac, sqlite3* DB);
//void handle_set_balance(AccountCollection& ac, sqlite3* DB);
//void handle_transfer_account(AccountCollection& ac, sqlite3* DB);
//void handle_transfer_cards(const AccountCollection& ac, sqlite3* DB);
//void handle_test_assignment_operator(const AccountCollection& ac, sqlite3* DB);
//void handle_test_equality_operator(const AccountCollection& ac);