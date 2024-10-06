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

    void save_cards_to_db(sqlite3* db) {
        const char* sql = "DELETE FROM accounts;";
        sqlite3_exec(db, sql, nullptr, nullptr, nullptr);

        for (const auto& card : cards_) {
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
        const char* sql_select = "SELECT id, card_number, expire_date, balance, account_id FROM cards WHERE account_id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id_);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* card_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* expire_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int card_balance = sqlite3_column_int(stmt, 3);
            int account_id = sqlite3_column_int(stmt, 4);

            std::unique_ptr<Card> card = std::make_unique<Card>(id, card_number, expire_date, card_balance, account_id);
            cards_.push_back(std::move(card));
        }

        sqlite3_finalize(stmt);
    }

    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};

class AccountCollection
{
private:
    std::vector<std::unique_ptr<Account>> accounts_;
public:
    void create_account(const int id,
        const std::string& client_name,
        const int balance)
    {
        if (find_account_by_id(id) != nullptr) {
            std::cout << "Account with ID " << id << " already exists" << std::endl;
            return;
        }

        auto account = std::make_unique<Account>(id, client_name, balance);
        accounts_.push_back(std::move(account));
    }

    Account* find_account_by_id(int id) {
        auto it = std::find_if(accounts_.begin(), accounts_.end(),
            [id](const auto& account) {
                return account->get_id() == id;
            });

        return it != accounts_.end() ? it->get() : nullptr;
    }

    void print_accounts()
    {
        for (auto& account : accounts_)
        {
            account->display_info();
        }
    }

    bool delete_account(const int id)
    {
        auto it = std::find_if(accounts_.begin(), accounts_.end(),
            [id](const auto& account) {
                return account->get_id() == id;
            });
       
        if (it != accounts_.end())
        {
            accounts_.erase(it);
            return true;
        }

        std::cout << "There is no account with ID = " << id << std::endl;
        return false;
    }

    void save_to_db(sqlite3* db)
    {
        const char* sql = "DELETE FROM accounts;";
        sqlite3_exec(db, sql, nullptr, nullptr, nullptr);

        for (const auto& account : accounts_) {
            const char* sql_account = "INSERT OR REPLACE INTO accounts (id, client_name, card_balance) VALUES (?, ?, ?);";
            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(db, sql_account, -1, &stmt, nullptr);

            sqlite3_bind_int(stmt, 1, account->get_id());
            sqlite3_bind_text(stmt, 2, account->get_name().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, account->get_balance());

            sqlite3_step(stmt);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_finalize(stmt);
            account->save_cards_to_db(db);
        }
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    }

    void load_from_db(sqlite3* db) {
        const char* sql_select = "SELECT id, client_name, card_balance FROM accounts;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* client_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int balance = sqlite3_column_int(stmt, 2);

            create_account(id, client_name, balance);
            accounts_.back()->load_cards_from_db(db);
        }

        sqlite3_finalize(stmt);
    }

    Account* operator[](int id) {
        return find_account_by_id(id);
    }

    void transfer_money_between_cards(const std::string_view& recipient_card_number,
        const std::string_view& sender_card_number, int sum)
    {
        Card* sender_card = find_card(*this, sender_card_number);
        if (sender_card && sender_card->get_balance() >= sum) {
            Card* recipient_card = find_card(*this, recipient_card_number);
            if (recipient_card) {
                sender_card->set_balance(sender_card->get_balance() - sum);
                recipient_card->set_balance(recipient_card->get_balance() + sum);
            }
            else {
                std::cout << "Recipient card not found: " << recipient_card_number << std::endl;
            }
        }
        else {
            std::cout << "Sender card not found or insufficient funds: " << sender_card_number << std::endl;
        }
    }

    void transfer_money_to_account_card(int account_id, const std::string_view& card_number, int sum)
    {
        Account* account = find_account_by_id(account_id);
        if (account && account->get_balance() >= sum) {
            Card* card = find_card(*this, card_number);
            if (card) {
                account->set_balance(account->get_balance() - sum);
                card->set_balance(card->get_balance() + sum);
            }
            else {
                std::cout << "Card not found: " << card_number << std::endl;
            }
        }
        else {
            std::cout << "Account not found or insufficient funds: " << account_id << std::endl;
        }
    }

    friend Card* find_card(const AccountCollection&, const std::string_view&);
};


