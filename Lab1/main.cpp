#include <iostream>
#include <memory>
#include "Account.h"
#include "menu.h"

int main()
{
    auto account = std::make_unique<Account>("Roman", 1000);
    menu(account);
    return 0;
}
