#include "menu_account_card.h"

void menu_account_card(sqlite3* DB)
{
    AccountCollection ac;
    ac.load_collection_from_db(DB);

    while (true)
    {
        display_menu();
        int choice;
        std::cin >> choice;
        std::cout << std::endl;

        using enum menu_options;

        switch (static_cast<menu_options>(choice))
        {
        case accounts_info:
            handle_accounts_info(ac);
            break;
        case add_account:
            handle_add_account(ac, DB);
            break;
        case delete_account:
            handle_delete_account(ac, DB);
            break;
        case add_card:
            handle_add_card(ac, DB);
            break;
        case delete_card:
            handle_delete_card(ac, DB);
            break;
        case set_name:
            handle_set_name(ac, DB);
            break;
        case set_balance:
            handle_set_balance(ac, DB);
            break;
        case transfer_account:
            handle_transfer_account(ac, DB);
            break;
        case transfer_cards:
            handle_transfer_cards(ac, DB);
            break;
        case test_assignment_operator:
            handle_test_assignment_operator(ac);
            break;
        case test_equality_operator:
            handle_test_equality_operator(ac);
            break;
        case end:
            std::cout << "Exit" << std::endl;
            return;
        default:
            std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}

void display_menu()
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
    std::cout << "Your option: ";
}

void handle_accounts_info(const AccountCollection& ac)
{
    ac.print_accounts();
}

void handle_add_account(AccountCollection& ac, sqlite3* DB)
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
    ac[id]->save_to_db(DB);
}

void handle_delete_account(AccountCollection& ac, sqlite3* DB)
{
    int id;
    std::cout << "Enter id of account to delete: ";
    std::cin >> id;

    if (ac[id])
    {
        ac[id]->delete_from_db(DB);
        ac.delete_account(id);
    }
    else
    {
        std::cerr << "There is no account with ID= " << id << std::endl;
    }
}

void handle_add_card(AccountCollection& ac, sqlite3* DB)
{
    int id;
    int card_balance;
    int account_id;
    std::string card_number, expire_date;

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
        if (find_card(ac, id))
            throw CardException(std::format("Card with ID = {} already exists", id));
        if (!ac[account_id])
            throw CardException(std::format("There is no account with ID = {}", account_id));

        auto card = std::make_unique<Card>(id, card_number, expire_date, card_balance, account_id);
        card->save_to_db(DB);
        ac[account_id]->add_card(std::move(card));
    }
    catch (const CardException& e)
    {
        std::cerr << e.what() << std::endl << std::endl;
    }
}

void handle_delete_card(AccountCollection& ac, sqlite3* DB)
{
    int account_id;
    int card_id;

    std::cout << "Enter the ID of the account that the card belongs to: ";
    std::cin >> account_id;
    std::cout << "Enter card id to delete: ";
    std::cin >> card_id;

    if (auto card = find_card(ac, card_id))
    {
        card->delete_from_db(DB);
        ac[account_id]->delete_card(card_id);
        std::cout << "Card was deleted" << std::endl << std::endl;
    }
    else
    {
        std::cout << "Card was not found" << std::endl << std::endl;
    }
}

void handle_set_name(AccountCollection& ac, sqlite3* DB)
{
    int account_id;
    std::string name;
    std::cout << "Enter account id: ";
    std::cin >> account_id;
    std::cout << "Enter new name: ";
    std::cin >> name;

    auto account = ac.find_account_by_id(account_id);
    account->set_name(name);
    ac.save_collection_to_db(DB);
}

void handle_set_balance(AccountCollection& ac, sqlite3* DB)
{
    int account_id;
    int balance;
    std::cout << "Enter account id: ";
    std::cin >> account_id;
    std::cout << "Enter new balance: ";
    std::cin >> balance;

    auto account = ac.find_account_by_id(account_id);
    account->set_balance(balance);
    ac.save_collection_to_db(DB);
}

void handle_transfer_account(AccountCollection& ac, sqlite3* DB)
{
    int account_id;
    int sum;
    std::string card_number;

    std::cout << "Enter sender's account id: ";
    std::cin >> account_id;
    std::cout << "Enter recipient card number: ";
    std::cin >> card_number;
    std::cout << "Enter sum: ";
    std::cin >> sum;

    ac.transfer_money_to_account_card(account_id, card_number, sum);
    ac.save_collection_to_db(DB);
}

void handle_transfer_cards(const AccountCollection& ac, sqlite3* DB)
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
    ac.save_collection_to_db(DB);
}

void handle_test_assignment_operator(const AccountCollection& ac)
{
    std::string source_card;
    std::string destination_card;

    std::cout << "Enter first card ID to copy from: ";
    std::cin >> source_card;
    std::cout << "Enter second card ID to copy to: ";
    std::cin >> destination_card;

    if (const Card* card_from = find_card(ac, source_card);
        Card * card_to = find_card(ac, destination_card))
    {
        *card_to = *card_from;
        std::cout << "Card " << destination_card << " assigned the values from card " << source_card << std::endl;
    }
    else
    {
        std::cout << "One of the cards not found." << std::endl;
    }
}

void handle_test_equality_operator(const AccountCollection& ac)
{
    std::string source_card;
    std::string destination_card;

    std::cout << "Enter first card number to compare with: ";
    std::cin >> source_card;
    std::cout << "Enter second card number to compare with: ";
    std::cin >> destination_card;

    if (const Card* card_from = find_card(ac, source_card);
        const Card * card_to = find_card(ac, destination_card))
    {
        std::cout << "Cards are " << ((*card_from == *card_to) ? "equal" : "not equal") << std::endl;
    }
    else
    {
        std::cout << "One of the cards not found." << std::endl;
    }
}
