#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

class BankCard
{
private:
    std::string card_number_;
    std::string expire_date_;
    int card_balance_;
public:
    BankCard(const std::string& card_number, const std::string& expire_date, const int card_balance = 0)
        : card_number_(card_number), expire_date_(expire_date), card_balance_(card_balance)
    {
    }

    ~BankCard()
    {
        std::cout << "Card deleted: " << card_number_;
    }

    void display_card_info() const
    {
        std::cout << "Card number: " << card_number_ << std::endl;
        std::cout << "Expire date: " << expire_date_ << std::endl;
        std::cout << "Card balance: " << card_balance_ << std::endl;
    }

    void set_card_number(const std::string_view& card_number)
    {
        card_number_ = card_number;
    }

    void set_expire_date(const std::string_view& expire_date)
    {
        expire_date_ = expire_date;
    }

    void set_card_balance(const int card_balance)
    {
        card_balance_ = card_balance;
    }

    std::string get_card_number() const
    {
        return card_number_;
    }

    std::string get_expire_date() const
    {
        return expire_date_;
    }

    int get_card_balance() const
    {
        return card_balance_;
    }

    BankCard(const BankCard&) = delete;
    void operator = (const BankCard&) = delete;
};

class Account
{
private:
    std::string client_name_;
    std::vector<std::unique_ptr<BankCard>> cards_;
    int account_balance_;
public:
    Account(const std::string& client_name, const int account_balance = 0)
        : client_name_(client_name), account_balance_(account_balance)
    {
    }

    ~Account()
    {
        cards_.clear();
    }

    void display_account_info() const
    {
        std::cout << "Client name: " << client_name_ << std::endl;
        std::cout << "Account balance: " << account_balance_ << std::endl;

        if (!cards_.empty())
        {
            std::cout << "Attached cards: " << std::endl << std::endl;
            for (auto& card : cards_)
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

    std::string get_client_name() const
    {
        return client_name_;
    }

    void set_client_name(const std::string_view& client_name)
    {
        client_name_ = client_name;
    }

    int get_account_balance() const
    {
        return account_balance_;
    }

    void set_account_balance(const int account_balance)
    {
        account_balance_ = account_balance;
    }

    const BankCard* get_card(const std::string_view& card_number) const
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

    void add_bank_card(std::unique_ptr<BankCard> card)
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

    bool delete_bank_card(const std::string_view& card_number)
    {
        for (auto iter = cards_.begin(); iter != cards_.end(); iter++)
        {
            if ((*iter)->get_card_number() == card_number)
            {
                cards_.erase(iter);
                return true;
            }
        }

        return false;
    }

    int get_account_available_balance() const
    {
        int cards_balance = 0;
        int balance_available = 0;

        for (auto& card : cards_)
        {
            cards_balance += card->get_card_balance();
        }

        if (account_balance_ - cards_balance > 0)
        {
            balance_available = account_balance_ - cards_balance;
        }

        return balance_available;
    }

    Account(const Account&) = delete;
    void operator = (const Account&) = delete;
};