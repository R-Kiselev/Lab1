#pragma once
#include <string>
#include <string_view>
#include <iostream>

class BankCard
{
private:
    std::string card_number_;
    std::string expire_date_;
    int card_balance_;

public:
    BankCard(const std::string& card_number, const std::string& expire_date, const int card_balance = 0);
    ~BankCard();

    void display_card_info() const;
    void set_card_number(const std::string_view& card_number);
    void set_expire_date(const std::string_view& expire_date);
    void set_card_balance(const int card_balance);

    std::string get_card_number() const;
    std::string get_expire_date() const;
    int get_card_balance() const;

    BankCard(const BankCard&) = delete;
    void operator=(const BankCard&) = delete;
};
