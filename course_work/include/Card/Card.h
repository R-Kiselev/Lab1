#pragma once
#include <iostream>
#include <string>
#include <random>
#include <chrono>

class Card {
private:
    int id_;
    std::string number_;
    std::string expire_date_;
    int balance_;
    int account_id_;

public:
    Card() = default;
    Card(const std::string& number, const std::string& expire_date, const int balance, const int account_id);
    ~Card() = default;

    void set_id(const int id);
    int get_id() const;
    void set_number(const std::string& number);
    std::string get_number() const;
    void set_expire_date(const std::string& expire_date);
    std::string get_expire_date() const;
    void set_balance(const int balance);
    int get_balance() const;
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
