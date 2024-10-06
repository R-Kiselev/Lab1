#include "Account.h"

Account::Account(const int id, const std::string& client_name, const int account_balance)
    : id_(id), name_(client_name), balance_(account_balance)
{
}
Account::~Account()
{
    std::cout << "Deleting account " << id_ << " and its cards" << std::endl;
    for (const auto& card : cards_) {
        std::cout << "Card ID: " << card->get_id() << std::endl;
    }
    cards_.clear();
}

void Account::display_info() const
{
    std::cout << std::endl;
    std::cout << "Account Id: " << id_ << std::endl;
    std::cout << "Client name: " << name_ << std::endl;
    std::cout << "Account balance: " << balance_ << std::endl;

    if (!cards_.empty())
    {
        std::cout << "Attached cards: " << std::endl << std::endl;
        for (const auto& card : cards_)
        {
            card->display_info();
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "No cards are attached to this account." << std::endl << std::endl;
    }
}

int Account::get_id() const
{
    return id_;
}

std::string Account::get_name() const
{
    return name_;
}
int Account::get_balance() const
{
    return balance_;
}

void Account::set_id(const int id)
{
    id_ = id;
}

void Account::set_name(const std::string_view& client_name)
{
    name_ = std::string(client_name);
}
void Account::set_balance(const int account_balance)
{
    balance_ = account_balance;
}

Card* Account::get_card(const std::string_view& card_number) const
{
    for (const auto& card : cards_)
    {
        if (card->get_number() == card_number)
        {
            return card.get();
        }
    }
    return nullptr;
}
void Account::add_card(std::unique_ptr<Card> card)
{
    if (get_card(card->get_number()) == nullptr)
    {
        cards_.push_back(std::move(card));
    }
    else
    {
        std::cout << "Card is already exists" << std::endl << std::endl;
    }
}

bool Account::delete_card(const std::string_view& card_number)
{
    for (auto iter = cards_.begin(); iter != cards_.end(); ++iter)
    {
        if ((*iter)->get_number() == card_number)
        {
            cards_.erase(iter);
            return true;
        }
    }
    return false;
}

int Account::get_available_balance() const
{
    int cards_balance = 0;

    for (const auto& card : cards_)
    {
        cards_balance += card->get_balance();
    }

    return (balance_ - cards_balance > 0) ? balance_ - cards_balance : 0;
}
void Account::transfer_money(const std::string_view& recipient_card_number, const std::string_view& sender_card_number, const int sum) const
{
    Card* sender_card = get_card(sender_card_number);
    if (sender_card != nullptr)
    {
        if (sender_card->get_balance() >= sum)
        {
            Card* recipient_card = get_card(recipient_card_number);
            if (recipient_card != nullptr)
            {
                int new_balance_recipient = recipient_card->get_balance() + sum;
                recipient_card->set_balance(new_balance_recipient);

                int new_balance_sender = sender_card->get_balance() - sum;
                sender_card->set_balance(new_balance_sender);
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
            std::cout << "Balance: " << sender_card->get_balance() << std::endl << std::endl;
        }
    }
    else
    {
        std::cout << "There is no such card number : " << sender_card_number << std::endl << std::endl;
    }
}
void Account::transfer_money(const std::string_view& card_number, const int sum) const
{
    int available_balance = get_available_balance();
    if (available_balance >= sum)
    {
        Card* card = get_card(card_number);
        if (card != nullptr)
        {
            int new_balance = card->get_balance() + sum;
            card->set_balance(new_balance);
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