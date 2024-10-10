#include "BankCard.h"

static bool is_card_number(const std::string_view& card_number)
{
    std::vector<char> accepted_symbols = { '-', ' ' };

    for (char ch : card_number)
    {
        if (!std::isdigit(ch) && std::ranges::find(accepted_symbols.begin(), accepted_symbols.end(), ch) == accepted_symbols.end())
        {
            return false;
        }
    }
    return true;
}
static bool is_expire_date(const std::string_view& exp_date) {
    std::vector<char> accepted_symbols = { '-', '\\', '/' };
    int symb_count = 0;
    int digits_count = 0;

    for (char ch : exp_date) {
        if (std::isdigit(ch)) {
            digits_count++;
            continue;
        }

        if (std::ranges::find(accepted_symbols, ch) == accepted_symbols.end() || symb_count > 1) {
            return false;
        }
        else
            symb_count++;
    }

    return digits_count == 4;
}
Card::Card(const int id,
    const std::string& card_number,
    const std::string& expire_date,
    const int balance,
    const int account_id)
    : id_(id), balance_(balance), account_id_(account_id)
{
    if (is_card_number(card_number) && is_expire_date(expire_date))
    {
        number_ = card_number;
        expire_date_ = expire_date;
    }
    else
    {
        throw CardException("Wrong symbols in card number or expire date.");
    }
}

Card::~Card()
{
    std::cout << "Card ID: " << get_id() << std::endl;
}

void Card::display_info() const
{
    std::cout << "Id: " << id_ << std::endl;
    std::cout << "Card number: " << number_ << std::endl;
    std::cout << "Expire date: " << expire_date_ << std::endl;
    std::cout << "Card balance: " << balance_ << std::endl;
}

void Card::set_number(const std::string_view& card_number)
{
    if (is_card_number(card_number))
    {
        number_ = std::string(card_number);
    }
    else
    {
        std::cerr << "Wrong symbols in card number." << std::endl << std::endl;
    }
}
void Card::set_expire_date(const std::string_view& expire_date)
{
    if (is_card_number(expire_date))
    {
        expire_date_ = std::string(expire_date);
    }
    else
    {
        std::cerr << "Wrong symbols in expire date." << std::endl << std::endl;
    }
}
void Card::set_balance(const int card_balance)
{
    balance_ = card_balance;
}

void Card::set_id(const int id)
{
    id_ = id;
}

void Card::set_account_id(const int id)
{
    account_id_ = id;
}


std::string Card::get_number() const
{
    return number_;
}
std::string Card::get_expire_date() const
{
    return expire_date_;
}
int Card::get_balance() const
{
    return balance_;
}

int Card::get_id() const
{
    return id_;
}

int Card::get_account_id() const
{
    return account_id_;
}
void Card::update_object_db(int id, const std::string& card_number, const std::string& expire_date,
    const int balance, const int account_id, sqlite3* db) const
{
    const char* sql_update = "UPDATE cards SET card_number = ?, expire_date = ?, balance = ?, account_id = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql_update, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare update statement: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_bind_text(stmt, 1, card_number.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, expire_date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, balance);
    sqlite3_bind_int(stmt, 4, account_id);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute update statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
}
void Card::save_to_db(sqlite3* db) const
{
    const char* sql_insert =
        "INSERT INTO cards (id, card_number, expire_date, balance, account_id) "
        "VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, get_id());
    sqlite3_bind_text(stmt, 2,get_number().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3,get_expire_date().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, get_balance());
    sqlite3_bind_int(stmt, 5, get_account_id());
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
void Card::delete_from_db(sqlite3* db)
{
    const char* sql_delete = "DELETE FROM cards WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql_delete, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, get_id());
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}