#include "menu_account_card.h"

void menu_account_card(sqlite3* DB)
{

    AccountCollection ac;

    ac.load_from_db(DB);

    //ac.create_account(1, "Roman", 1000);
    //auto card1 = std::make_unique<Card>(1, "1111-2222-3333-4444", "08/27", 300, 1);
    //auto card2 = std::make_unique<Card>(2, "2222-3333-4444-6666", "08/27", 300, 1);
    //ac[1]->add_card(std::move(card1));
    //ac[1]->add_card(std::move(card2));
    //ac.create_account(2, "John", 500);
    //auto card3 = std::make_unique<Card>(3, "3333-4444-5555-6666", "09/28", 200, 2);
    //auto card4 = std::make_unique<Card>(4, "4444-5555-6666-7777", "09/28", 200, 2);
    //ac[2]->add_card(std::move(card3));
    //ac[2]->add_card(std::move(card4));
    //ac.create_account(3, "Emily", 2000);
    //auto card5 = std::make_unique<Card>(5, "5555-6666-7777-8888", "10/29", 500, 3);
    //auto card6 = std::make_unique<Card>(6, "6666-7777-8888-9999", "10/29", 500, 3);
    //ac[3]->add_card(std::move(card5));
    //ac[3]->add_card(std::move(card6));

    while (true)
    {
        std::cout << "1) Display accounts info" << std::endl;
        std::cout << "2) Add account" << std::endl;
        std::cout << "3) Delete account" << std::endl;

        std::cout << "4) Add card" << std::endl;
        std::cout << "5) Delete card" << std::endl;
        
        std::cout << "6) Set account name" << std::endl;
        std::cout << "7) Set account balance" << std::endl;
        
        std::cout << "8) Transfer money between account and card" << std::endl;
        std::cout << "9) Transfer money between two cards" << std::endl;

        std::cout << "10) Save info to the database" << std::endl;

        std::cout << "0) Exit" << std::endl;
        std::cout << "Your option : ";

        int choice;
        std::cin >> choice;
        std::cout << std::endl;

        auto option = static_cast<menu_options>(choice);
        switch (option)
        {
        case menu_options::accounts_info:
            ac.print_accounts();
            break;
        case menu_options::add_account:
        {
            int id;
            int balance;
            std::string client_name;

            std::cout << "Enter id: ";
            std::cin >> id;
            std::cout << "Enter client name: ";
            std::cin >> client_name;
            std::cout << "Enter balance: ";
            std::cin >> balance;

            ac.create_account(id, client_name, balance);
            break;
        }
        case menu_options::delete_account:
        {
            int id;
            std::cout << "Enter id of account to delete: ";
            std::cin >> id;

            ac.delete_account(id);

            break;
        }
        case menu_options::add_card:
        {
            int id;
            std::string card_number;
            std::string expire_date;
            int card_balance;
            int account_id;

            std::cout << "Enter card id: ";
            std::cin >> id;
            std::cout << "Enter card number: ";
            std::cin >> card_number;
            std::cout << "Enter expire date (MM/YY): ";
            std::cin >> expire_date;
            std::cout << "Enter card balance: ";
            std::cin >> card_balance;

            std::cout << "Enter account id: ";
            std::cin >> account_id;

            try
            {
                std::unique_ptr<Card> card = std::make_unique<Card>(id, card_number, expire_date, card_balance, account_id);
                ac[account_id]->add_card(std::move(card));
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl << std::endl;
            }

            break;
        }
        case menu_options::delete_card:
        {
            int account_id;
            std::string card_number;

            std::cout << "Enter the ID of the account that the card belongs to: ";
            std::cin >> account_id;
            std::cout << "Enter card number to delete: ";
            std::cin >> card_number;

            if (ac[account_id]->delete_card(card_number))
            {
                std::cout << "Card was deleted" << std::endl << std::endl;
            }
            else
            {
                std::cout << "Card was not found" << std::endl << std::endl;
            }
            break;
        }
        case menu_options::set_name:
        {
            int account_id;
            std::string name;
            std::cout << "Enter account id: ";
            std::cin >> account_id;
            std::cout << "Enter new name: ";
            std::cin >> name;

            auto account = ac.find_account_by_id(account_id);
            account->set_name(name);

            break;
        }
        case menu_options::set_balance:
        {
            int account_id;
            int balance;
            std::cout << "Enter account id: ";
            std::cin >> account_id;
            std::cout << "Enter new balance: ";
            std::cin >> balance;

            auto account = ac.find_account_by_id(account_id);
            account->set_balance(balance);
            break;
        }
        case menu_options::transfer_account:
        {
            int account_id;
            std::string card_number;
            int sum;

            std::cout << "Enter sender's account id: ";
            std::cin >> account_id;
            std::cout << "Enter recipient card number: ";
            std::cin >> card_number;
            std::cout << "Enter sum: ";
            std::cin >> sum;

            ac.transfer_money_to_account_card(account_id, card_number, sum);
            break;
        }
        case menu_options::transfer_cards:
        {
            std::string recipient_card;
            std::string sender_card;
            int sum;

            std::cout << "Enter recipient card number: ";
            std::cin >> recipient_card;
            std::cout << "Enter sender card number: ";
            std::cin >> sender_card;
            std::cout << "Enter sum: ";
            std::cin >> sum;

            ac.transfer_money_between_cards(recipient_card, sender_card, sum);
            break;
        }
        case menu_options::save_into_db:
            ac.save_to_db(DB);
            break;
        case menu_options::end:
            std::cout << "Exit" << std::endl;
            return;
        default:
            std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}