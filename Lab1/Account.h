#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "BankCard.h"

class Account
{
private:
    std::string client_name_;
    std::vector<std::unique_ptr<BankCard>> cards_;
    int account_balance_;

public:
    Account(const std::string& client_name, const int account_balance = 0);
    ~Account();

    void display_account_info() const;
    std::string get_client_name() const;
    void set_client_name(const std::string_view& client_name);
    int get_account_balance() const;
    void set_account_balance(const int account_balance);

    const BankCard* get_card(const std::string_view& card_number) const;
    void add_bank_card(std::unique_ptr<BankCard> card);
    bool delete_bank_card(const std::string_view& card_number);
    int get_account_available_balance() const;

    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};
