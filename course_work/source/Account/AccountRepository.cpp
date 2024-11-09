#include "../../include/Account/AccountRepository.h"

void AccountRepository::add(Account* account) const
{
    std::string sql = "INSERT INTO accounts (client_id, bank_id) VALUES (?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for adding account");
    }
    sqlite3_bind_int(stmt, 1, account->get_client_id());
    sqlite3_bind_int(stmt, 2, account->get_bank_id());
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to insert client into database");
    }

    sqlite3_finalize(stmt);
}
std::unique_ptr<Account> AccountRepository::get_by_id(const int id) const
{
    std::string sql = "SELECT id, balance, client_id, bank_id FROM accounts WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting account by ID");
    }
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException("Account with ID " + std::to_string(id) + " not found in database");
    }
    int account_id = sqlite3_column_int(stmt, 0);
    int balance = sqlite3_column_int(stmt, 1);
    int client_id = sqlite3_column_int(stmt, 2);
    int bank_id = sqlite3_column_int(stmt, 3);

    auto account = std::make_unique<Account>(client_id, bank_id);
    account->set_id(account_id);
    account->set_balance(balance);

    return account;
}
std::vector<std::unique_ptr<Account>> AccountRepository::get_all() const {
    std::vector<std::unique_ptr<Account>> accounts;
    std::string sql = "SELECT id, balance, client_id, bank_id FROM accounts;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting all accounts");
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto account = std::make_unique<Account>();

        account->set_id(sqlite3_column_int(stmt, 0));
        account->set_balance(sqlite3_column_int(stmt, 1));
        account->set_client_id(sqlite3_column_int(stmt, 2));
        account->set_bank_id(sqlite3_column_int(stmt, 3));

        accounts.push_back(std::move(account));
    }
    sqlite3_finalize(stmt);
    if (accounts.empty()) {
        throw NotFoundException("No accounts found in database");
    }
    return accounts;
}
void AccountRepository::update(Account *account) const {
    std::string sql = "UPDATE accounts SET balance = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        throw DatabaseException("Failed to prepare SQL statement for updating account");
    }
    sqlite3_bind_int(stmt, 1, account->get_balance());
    sqlite3_bind_int(stmt, 2, account->get_id());
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to update account in database");
    }

    sqlite3_finalize(stmt);
}
void AccountRepository::remove(const int id) {
    std::string sql = "DELETE FROM accounts WHERE id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr)!= SQLITE_OK){
        throw DatabaseException("Failed to prepare SQL statement for deleting account");
    }
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to delete account from database");
    }
    sqlite3_finalize(stmt);
}
bool AccountRepository::exists(const int id) const
{
    std::string sql = "SELECT EXISTS(SELECT 1 FROM accounts WHERE id = ?);";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking account existence");
    }

    sqlite3_bind_int(stmt, 1, id);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return exists;
}
std::vector<std::unique_ptr<Account>> AccountRepository::get_all_by_client_id(int client_id) const
{
    std::string sql = "SELECT id, balance, client_id, bank_id FROM accounts WHERE client_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking account existence");
    }
    sqlite3_bind_int(stmt, 1, client_id);
    std::vector<std::unique_ptr<Account>> accounts;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto account = std::make_unique<Account>();

        account->set_id(sqlite3_column_int(stmt, 0));
        account->set_balance(sqlite3_column_int(stmt, 1));
        account->set_client_id(sqlite3_column_int(stmt, 2));
        account->set_bank_id(sqlite3_column_int(stmt, 3));

        accounts.push_back(std::move(account));
    }
    sqlite3_finalize(stmt);
    if (accounts.empty()) {
        throw NotFoundException("No accounts found in database");
    }
    return accounts;
}
