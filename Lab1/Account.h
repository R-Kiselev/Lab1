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

    void save_cards_to_db(sqlite3* db) const
    {
        for (const auto& card : cards_) 
        {
            const char* sql_insert =
                "INSERT OR REPLACE INTO cards (id, card_number, expire_date, balance, account_id) "
                "VALUES (?, ?, ?, ?, ?);";
            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
            sqlite3_bind_int(stmt, 1, card->get_id());
            sqlite3_bind_text(stmt, 2, card->get_number().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, card->get_expire_date().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 4, card->get_balance());
            sqlite3_bind_int(stmt, 5, card->get_account_id());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    void load_cards_from_db(sqlite3* db) {
        auto* sql_select = "SELECT id, card_number, expire_date, balance, account_id FROM cards WHERE account_id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id_);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            auto* card_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            auto* expire_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int card_balance = sqlite3_column_int(stmt, 3);
            int account_id = sqlite3_column_int(stmt, 4);

            auto card = std::make_unique<Card>(id, card_number, expire_date, card_balance, account_id);
            cards_.push_back(std::move(card));
        }

        sqlite3_finalize(stmt);
    }

    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};