#pragma once
#include <string>
#include <memory>
#include "Account.h"

enum MenuOptions
{
    End = 0,
    AccountInfo,
    AddCard,
    DeleteCard,
    GetBalance,
    SetName,
    SetBalance,
};

void menu(std::unique_ptr<Account>& account);
void handle_account_info(const std::unique_ptr<Account>& account);
void handle_add_card(std::unique_ptr<Account>& account);
void handle_delete_card(std::unique_ptr<Account>& account);
void handle_get_balance(const std::unique_ptr<Account>& account);
void handle_set_name(std::unique_ptr<Account>& account);
void handle_set_balance(std::unique_ptr<Account>& account);
