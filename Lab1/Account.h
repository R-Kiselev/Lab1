#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "BankCard.h"
#include <sqlite3.h>

class Account
{
private:
    int id_;
    std::string name_;
    int balance_;

    std::vector<std::unique_ptr<Card>> cards_;

public:
    Account(const int id, const std::string& name, const int balance = 0);
    ~Account();

    void display_info() const;
    int get_id() const;
    void set_id(const int id);
    std::string get_name() const;
    void set_name(const std::string_view& name);
    int get_balance() const;
    void set_balance(const int balance);

    Card* get_card(const std::string_view& card_number) const;
    void add_card(std::unique_ptr<Card> card);
    bool delete_card(const std::string_view& card_number);
    int get_available_balance() const;
    
    void transfer_money(const std::string_view& recipient_card_number, const std::string_view& sender_card_number, const int sum) const;

    void transfer_money(const std::string_view& card_number, const int sum) const;

    void save_cards_to_db(sqlite3* db) const;
    void load_cards_from_db(sqlite3* db);

    
    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};