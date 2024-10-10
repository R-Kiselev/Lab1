#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
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