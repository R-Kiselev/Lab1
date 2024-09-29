#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include "Account.h"

enum class menu_options
{
    end = 0,
    account_info,
    add_card,
    delete_card,
    get_balance,
    set_name,
    set_balance,
    transfer_account,
    transfer_cards
};

void menu_account_card();