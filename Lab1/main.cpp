#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include "Account.h"
#include "main.h"

int main()
{
    auto account = std::make_unique<Account>("Roman", 1000);
    while (true)
    {
        std::cout << "1) Display account info" << std::endl;
        std::cout << "2) Add bank card" << std::endl;
        std::cout << "3) Delete bank card" << std::endl;
        std::cout << "4) Get available account balance" << std::endl;
        std::cout << "5) Set account name" << std::endl;
        std::cout << "6) Set account balance" << std::endl;
        std::cout << "0) Exit" << std::endl;
        std::cout << "Your option : ";

        int choice;
        std::cin >> choice;

        auto option = static_cast<menu_options>(choice);
        switch (option)
        {
        case menu_options::account_info:
            account->display_account_info();
            break;
        case menu_options::add_card:
        {
            std::string card_number;
            std::string expire_date;

            int card_balance;
            std::cout << "Enter card number: ";
            std::cin >> card_number;
            std::cout << "Enter expire date (MM/YY): ";
            std::cin >> expire_date;
            std::cout << "Enter card balance: ";
            std::cin >> card_balance;
            account->add_bank_card(std::make_unique<BankCard>(card_number, expire_date, card_balance));
            break;
        }
        case menu_options::delete_card:
        {
            std::string card_number;
            std::cout << "Enter card number to delete: ";
            std::cin >> card_number;

            if (account->delete_bank_card(card_number))
            {
                std::cout << "Card was deleted" << std::endl;
            }
            else
            {
                std::cout << "Card was not found" << std::endl;
            }

            break;
        }
        case menu_options::get_balance:
            std::cout << "Available account balance: " << account->get_account_available_balance() << std::endl;
            break;
        case menu_options::set_name:
        {
            std::string new_name;
            std::cout << "Enter new account name: ";
            std::cin >> new_name;
            account->set_client_name(new_name);
            break;
        }
        case menu_options::set_balance:
        {
            int new_balance;
            std::cout << "Enter new account balance: ";
            std::cin >> new_balance;
            account->set_account_balance(new_balance);
            break;
        }
        case menu_options::end:
            std::cout << "Exit" << std::endl;
            return 0;
        default:
            std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}