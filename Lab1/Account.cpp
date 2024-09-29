#include "Account.h"

Account::Account(const std::string& client_name, const int account_balance)
    : client_name_(client_name), account_balance_(account_balance)
{
}

Account::~Account()
{
    cards_.clear();
}

void Account::display_account_info() const
{
    std::cout << "Client name: " << client_name_ << std::endl;
    std::cout << "Account balance: " << account_balance_ << std::endl;

    if (!cards_.empty())
    {
        std::cout << "Attached cards: " << std::endl << std::endl;
        for (const auto& card : cards_)
        {
            card->display_card_info();
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "No cards are attached to this account." << std::endl << std::endl;
    }
}

std::string Account::get_client_name() const
{
    return client_name_;
}

void Account::set_client_name(const std::string_view& client_name)
{
    client_name_ = std::string(client_name);
}

int Account::get_account_balance() const
{
    return account_balance_;
}

void Account::set_account_balance(const int account_balance)
{
    account_balance_ = account_balance;
}

BankCard* Account::get_card(const std::string_view& card_number) const
{
    for (const auto& card : cards_)
    {
        if (card->get_card_number() == card_number)
        {
            return card.get();
        }
    }
    return nullptr;
}

void Account::add_bank_card(std::unique_ptr<BankCard> card)
{
    if (get_card(card->get_card_number()) == nullptr)
    {
        cards_.push_back(std::move(card));
    }
    else
    {
        std::cout << "Card is already exists" << std::endl << std::endl;
    }
}

bool Account::delete_bank_card(const std::string_view& card_number)
{
    for (auto iter = cards_.begin(); iter != cards_.end(); ++iter)
    {
        if ((*iter)->get_card_number() == card_number)
        {
            cards_.erase(iter);
            return true;
        }
    }
    return false;
}

int Account::get_account_available_balance() const
{
    int cards_balance = 0;

    for (const auto& card : cards_)
    {
        cards_balance += card->get_card_balance();
    }

    return (account_balance_ - cards_balance > 0) ? account_balance_ - cards_balance : 0;
}

void Account::transfer_money(const std::string_view& recipient_card_number, const std::string_view& sender_card_number, const int sum) const
{
    BankCard* sender_card = get_card(sender_card_number);
    if (sender_card != nullptr)
    {
        if (sender_card->get_card_balance() >= sum)
        {
            BankCard* recipient_card = get_card(recipient_card_number);
            if (recipient_card != nullptr)
            {
                int new_balance_recipient = recipient_card->get_card_balance() + sum;
                recipient_card->set_card_balance(new_balance_recipient);

                int new_balance_sender = sender_card->get_card_balance() - sum;
                sender_card->set_card_balance(new_balance_sender);
            }
            else
            {
                std::cout << "There is no such card number : " << recipient_card_number << std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "Sum is greater than sender's card balance " << std::endl;
            std::cout << "Sum: " << sum << std::endl;
            std::cout << "Balance: " << sender_card->get_card_balance() << std::endl << std::endl;
        }
    }
    else
    {
        std::cout << "There is no such card number : " << sender_card_number << std::endl << std::endl;
    }
}
void Account::transfer_money(const std::string_view& card_number, const int sum) const
{
    int available_balance = get_account_available_balance();
    if (available_balance >= sum)
    {
        BankCard* card = get_card(card_number);
        if (card != nullptr)
        {
            int new_balance = card->get_card_balance() + sum;
            card->set_card_balance(new_balance);
        }
        else
        {
            std::cout << "There is no such card number : " << card_number << std::endl << std::endl;
        }
    }
    else
    {
        std::cout << "There is no available account balance" << std::endl << std::endl;
    }
}