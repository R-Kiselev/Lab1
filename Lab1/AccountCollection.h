#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include "Account.h"
#include "BankCard.h"

class AccountCollection : public IDatabaseCollection
{
private:
    std::vector<std::unique_ptr<Account>> accounts_;

public:
    void create_account(const int id, const std::string& client_name, const int balance);
    Account* find_account_by_id(int id);
    void print_accounts() const;
    bool delete_account(const int id);
   
    void transfer_money_between_cards(const std::string_view& recipient_card_number,
        const std::string_view& sender_card_number, int sum) const;
    void transfer_money_to_account_card(int account_id, const std::string_view& card_number, int sum);
    
    void save_collection_to_db(sqlite3* db) const override;
    void load_collection_from_db(sqlite3* db) override;

    Account* operator[](int id);

    friend Card* find_card(const AccountCollection&, const int id);
    friend Card* find_card(const AccountCollection& collection, const std::string_view& card_number);
};
