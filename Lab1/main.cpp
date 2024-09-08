#include <iostream>
#include <memory>
#include "Account.h"
#include "Menu.h"

int main()
{
    auto account = std::make_unique<Account>("Roman", 1000);
    menu(account);
    return 0;
}
