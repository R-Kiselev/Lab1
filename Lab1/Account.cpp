#include "Account.h"

Account::Account(const int id, const std::string& client_name, const int account_balance)
    : id_(id), name_(client_name), balance_(account_balance)
{
}
Account::~Account()
{
    std::cout << "Deleting account " << id_ << std::endl;
    cards_.clear();
}

void Account::display_info() const
{
    std::cout << std::endl;
    std::cout << "Account Id: " << id_ << std::endl;
    std::cout << "Client name: " << name_ << std::endl;
    std::cout << "Account balance: " << balance_ << std::endl;

    if (!cards_.empty())
    {
        std::cout << "Attached cards: " << std::endl << std::endl;
        for (const auto& card : cards_)
        {
            card->display_info();
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "No cards are attached to this account." << std::endl << std::endl;
    }
}

int Account::get_id() const
{
    return id_;
}

std::string Account::get_name() const
{
    return name_;
}
int Account::get_balance() const
{
    return balance_;
}

void Account::set_id(const int id)
{
    id_ = id;
}

void Account::set_name(const std::string_view& client_name)
{
    name_ = std::string(client_name);
}
void Account::set_balance(const int account_balance)
{
    balance_ = account_balance;
}

Card* Account::get_card(const std::string_view& card_number) const
{
    for (const auto& card : cards_)
    {
        if (card->get_number() == card_number)
        {
            return card.get();
        }
    }
    return nullptr;
}

Card* Account::get_card(const int id) const
{
    for (const auto& card : cards_)
    {
        if (card->get_id() == id)
        {
            return card.get();
        }
    }
    return nullptr;
}

void Account::add_card(std::unique_ptr<Card> card)
{
    if (get_card(card->get_number()) == nullptr)
    {
        if (card->get_account_id() != get_id())
        {
            std::cerr << "Error";
        }
        cards_.push_back(std::move(card));
    }
    else
    {
        std::cout << "Card is already exists" << std::endl << std::endl;
    }
}

bool Account::delete_card(const int id)
{
    for (auto iter = cards_.begin(); iter != cards_.end(); ++iter)
    {
        if ((*iter)->get_id() == id)
        {
            cards_.erase(iter);
            return true;
        }
    }
    return false;
}

int Account::get_available_balance() const
{
    int cards_balance = 0;

    for (const auto& card : cards_)
    {
        cards_balance += card->get_balance();
    }

    return (balance_ - cards_balance > 0) ? balance_ - cards_balance : 0;
}

void Account::save_collection_to_db(sqlite3* db) const
{
    for (const auto& card : cards_)
    {
        card->save_to_db(db);
    }
}

void Account::load_collection_from_db(sqlite3* db)
{
    const char* sql_select = "SELECT id, card_number, expire_date, balance, account_id FROM cards WHERE account_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id_);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
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

void Account::save_to_db(sqlite3* db) const
{
    std::string sql_insert =
        "INSERT INTO accounts (id, client_name, card_balance) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql_insert.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        exit(-2);
    }

    sqlite3_bind_int(stmt, 1, get_id());
    sqlite3_bind_text(stmt, 2,get_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, get_balance());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    save_collection_to_db(db);
}
void Account::delete_from_db(sqlite3* db)
{
    const char* sql_delete = "DELETE FROM accounts WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql_delete, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, get_id());
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    for (const auto& card : cards_)
    {
        card->delete_from_db(db);
    }
}