#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include "interfaces.h"

class Card : public IDatabaseObject, public IFinancial
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
    void set_balance(const int card_balance) override;
    void set_id(const int id);
    void set_account_id(const int id);

    std::string get_number() const;
    std::string get_expire_date() const;
    int get_balance() const override;
    int get_id() const;
    int get_account_id() const;

    void save_to_db(sqlite3* db) const override;
    void delete_from_db(sqlite3* db) override;

    Card(const Card&) = delete;
    void operator=(const Card& other) 
    {
        if (this != &other) 
        {
            expire_date_ = other.expire_date_;
            balance_ = other.balance_;
        }
    }
    bool operator==(const Card& other) const 
    {
        return balance_ == other.balance_
            && expire_date_ == other.expire_date_;
    }
};

class CardException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};