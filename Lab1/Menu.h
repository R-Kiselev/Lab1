#pragma once
#include <string>
#include <memory>
#include "Account.h"

enum menu_options
{
    exit_program = 0,
    account_info,
    add_card,
    delete_card,
    get_balance,
    set_name,
    set_balance,
};

void menu(std::unique_ptr<Account>& account);
void handle_account_info(const std::unique_ptr<Account>& account);
void handle_add_card(const std::unique_ptr<Account>& account);
void handle_delete_card(const std::unique_ptr<Account>& account);
void handle_get_balance(const std::unique_ptr<Account>& account);
void handle_set_name(const std::unique_ptr<Account>& account);
void handle_set_balance(const std::unique_ptr<Account>& account);
