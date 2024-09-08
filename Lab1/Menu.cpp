#include "menu.h"
#include <iostream>
#include <string>
#include <memory>

void menu(std::unique_ptr<Account>& account)
{
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

        switch (choice)
        {
        case AccountInfo:
            handle_account_info(account);
            break;
        case AddCard:
            handle_add_card(account);
            break;
        case DeleteCard:
            handle_delete_card(account);
            break;
        case GetBalance:
            handle_get_balance(account);
            break;
        case SetName:
            handle_set_name(account);
            break;
        case SetBalance:
            handle_set_balance(account);
            break;
        case End:
            std::cout << "Exit" << std::endl;
            return;
        default:
            std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}

void handle_account_info(const std::unique_ptr<Account>& account)
{
    account->display_account_info();
}

void handle_add_card(std::unique_ptr<Account>& account)
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
}

void handle_delete_card(std::unique_ptr<Account>& account)
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
}

void handle_get_balance(const std::unique_ptr<Account>& account)
{
    std::cout << "Available account balance: " << account->get_account_available_balance() << std::endl;
}

void handle_set_name(std::unique_ptr<Account>& account)
{
    std::string new_name;
    std::cout << "Enter new account name: ";
    std::cin >> new_name;
    account->set_client_name(new_name);
}

void handle_set_balance(std::unique_ptr<Account>& account)
{
    int new_balance;
    std::cout << "Enter new account balance: ";
    std::cin >> new_balance;
    account->set_account_balance(new_balance);
}
