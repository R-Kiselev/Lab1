#include "menu_account_card.h"

void menu_account_card(sqlite3* DB)
{
    AccountCollection ac;

    ac.load_from_db(DB);

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

        std::cout << "10) Test assignment operator" << std::endl;
        std::cout << "11) Test equality operator" << std::endl;

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
            ac.save_to_db(DB);
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
                auto card = std::make_unique<Card>(id, card_number, expire_date, card_balance, account_id);
                if (ac[account_id])
                {
                    ac[account_id]->add_card(std::move(card));
                }
                else
                {
                    throw CardException("There is no account with ID = " + std::to_string(account_id));
                }
            }
            catch (const CardException& e)
            {
                std::cerr << e.what() << std::endl << std::endl;
            }
            ac.save_to_db(DB);
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
            ac.save_to_db(DB);
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

            ac.save_to_db(DB);
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
            ac.save_to_db(DB);
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
            ac.save_to_db(DB);
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
            ac.save_to_db(DB);
            break;
        }
        case menu_options::test_assignment_operator:
        {
            std::string source_card;
            std::string destination_card;

            std::cout << "Enter first card ID to copy from: ";
            std::cin >> source_card;
            std::cout << "Enter second card ID to copy to: ";
            std::cin >> destination_card;

            if (const Card* card_from = find_card(ac, source_card);
                Card* card_to = find_card(ac, destination_card)) {
                *card_to = *card_from;
                std::cout << "Card " << destination_card << " assigned the values from card " << source_card << std::endl;
            }
            else {
                std::cout << "One of the cards not found." << std::endl;
            }
            ac.save_to_db(DB);
            break;
        }
        case menu_options::test_equality_operator:
        {
            std::string source_card;
            std::string destination_card;

            std::cout << "Enter first card number to compare with: ";
            std::cin >> source_card;
            std::cout << "Enter second card number to compare with: ";
            std::cin >> destination_card;

            if (const Card* card_from = find_card(ac, source_card);
                const Card* card_to = find_card(ac, destination_card)) {
                (*card_from == *card_to) ?
                    std::cout << "Cards " << source_card << " and " << destination_card << " are equal in balance and expire date." << std::endl
                :
                    std::cout << "Cards " << source_card << " and " << destination_card << " are not equal." << std::endl;
                
            }
            else {
                std::cout << "One of the cards not found." << std::endl;
            }
            break;
        }
        case menu_options::end:
            std::cout << "Exit" << std::endl;
            return;
        default:
            std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}