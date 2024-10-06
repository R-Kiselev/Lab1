#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <sqlite3.h>

class Card
{
private:
    int id_;
    std::string number_;
    std::string expire_date_;
    int balance_;
    int account_id_;

public:
    Card(const int id,
        const std::string& number,
        const std::string& expire_date,
        const int card_balance,
        const int account_id);
    ~Card();

    void display_info() const;
    void set_number(const std::string_view& number);
    void set_expire_date(const std::string_view& expire_date);
    void set_balance(const int card_balance);
    void set_id(const int id);
    void set_account_id(const int id);

    std::string get_number() const;
    std::string get_expire_date() const;
    int get_balance() const;
    int get_id() const;
    int get_account_id() const;

    Card(const Card&) = delete;
    void operator=(const Card&) = delete;
};

