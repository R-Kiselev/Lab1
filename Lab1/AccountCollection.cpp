#include "AccountCollection.h"
#include <iostream>
#include <algorithm>

void AccountCollection::create_account(const int id, const std::string& client_name, const int balance) {
    if (find_account_by_id(id) != nullptr) {
        std::cout << "Account with ID " << id << " already exists" << std::endl;
        return;
    }

    auto account = std::make_unique<Account>(id, client_name, balance);
    accounts_.push_back(std::move(account));
}

Account* AccountCollection::find_account_by_id(int id) {
    auto it = std::ranges::find_if(accounts_,
        [id](const std::unique_ptr<Account>& account) {
            return account->get_id() == id;
        });

    return it != accounts_.end() ? it->get() : nullptr;
}

void AccountCollection::print_accounts() const {
    for (auto& account : accounts_) {
        account->display_info();
    }
}

bool AccountCollection::delete_account(const int id) {
    if (auto it = std::ranges::find_if(accounts_, [id](const auto& account) {
        return account->get_id() == id;
        }); it != accounts_.end()) {
        accounts_.erase(it);
        return true;
    }
    std::cout << "There is no account with ID = " << id << std::endl;
    return false;
}

void AccountCollection::save_collection_to_db(sqlite3* db) const
{
    for (const auto& account : accounts_) {
        account->save_to_db(db);
    }
}

void AccountCollection::load_collection_from_db(sqlite3* db) {
    const char* sql_select = "SELECT id, client_name, card_balance FROM accounts;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        auto* client_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int balance = sqlite3_column_int(stmt, 2);

        create_account(id, client_name, balance);
        accounts_.back()->load_collection_from_db(db);
    }

    sqlite3_finalize(stmt);
}

Account* AccountCollection::operator[](int id) {
    return find_account_by_id(id);
}

void AccountCollection::transfer_money_between_cards(const std::string_view& recipient_card_number,
    const std::string_view& sender_card_number, int sum) const
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

void AccountCollection::transfer_money_to_account_card(int account_id, const std::string_view& card_number, int sum) {
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

Card* find_card(const AccountCollection& collection, const int id) {
    for (const auto& account : collection.accounts_) {
        Card* card = account->get_card(id);
        if (card != nullptr) {
            return card;
        }
    }
    return nullptr;
}

Card* find_card(const AccountCollection& collection, const std::string_view& card_number) {
    for (const auto& account : collection.accounts_) {
        Card* card = account->get_card(card_number);
        if (card != nullptr) {
            return card;
        }
    }
    return nullptr;
}