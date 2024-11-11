#pragma once
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include "../interfaces.h"

class Card : public IEntity, public IFinancial
{
private:
    int id_;
    std::string number_;
    std::string expire_date_;
    int balance_;
    int account_id_;

public:
    Card() = default;
    Card(const std::string& number, const std::string& expire_date, const int balance, const int account_id);
    ~Card() override = default;

    int get_id() const override;
    void set_id(const int id) override;
    int get_balance() const override;
    void set_balance(const int balance) override;
    std::string get_number() const;
    void set_number(const std::string_view& number);
    std::string get_expire_date() const;
    void set_expire_date(const std::string_view& expire_date);
    void set_account_id(const int account_id);
    int get_account_id() const;

    Card(const Card&) = delete;
    void operator=(const Card&) = delete;

    bool operator==(const Card& other) const {
        return this->balance_ == other.balance_;
    }
    friend int operator+(const Card& lhs, const Card& rhs) {
        return lhs.balance_ + rhs.balance_;
    }
    friend int operator-(const Card& lhs, const Card& rhs) {
        return lhs.balance_ - rhs.balance_;
    }
};
