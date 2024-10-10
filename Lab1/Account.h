#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "BankCard.h"
#include <sqlite3.h>

class Account : public IFinancial, public IDatabaseObject, public IDatabaseCollection
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

    Card* get_card(const std::string_view& card_number) const;
    Card* get_card(const int id) const;

    void add_card(std::unique_ptr<Card> card);
    bool delete_card(const int id);
    int get_available_balance() const;
    
    int get_balance() const override;
    void set_balance(const int balance) override;

    void save_to_db(sqlite3* db) const override;
    void delete_from_db(sqlite3* db) override;

    void save_collection_to_db(sqlite3* db) const override;
    void load_collection_from_db(sqlite3* db) override;
    
    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};