#include "../../include/Bank/BankRepository.h"


void BankRepository::add(Bank* bank) const {
    std::string sql = "INSERT INTO banks (name) VALUES ('" + bank->get_name() + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error_message = errMsg;
        sqlite3_free(errMsg);
        throw DatabaseException(error_message);
    }
    int last_id = int(sqlite3_last_insert_rowid(db_));
    bank->set_id(last_id);
}

void BankRepository::remove(int id) {
    std::string sql = std::format("DELETE FROM banks WHERE id = {} ;", std::to_string(id));
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error_message = errMsg;
        sqlite3_free(errMsg);
        throw DatabaseException(error_message);
    }
}

std::unique_ptr<Bank> BankRepository::get_by_id(int id) const {
    std::string sql = std::format("SELECT id, name FROM banks WHERE id = {} ;", std::to_string(id));
    sqlite3_stmt* stmt;
    auto bank = std::make_unique<Bank>();

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        bank->set_id(sqlite3_column_int(stmt, 0));
        bank->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    } else {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Bank with ID {} not found", std::to_string(id)));
    }

    sqlite3_finalize(stmt);
    return bank;
}

std::vector<std::unique_ptr<Bank>> BankRepository::get_all() const {
    std::vector<std::unique_ptr<Bank>> banks;
    std::string sql = "SELECT id, name FROM banks;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto bank = std::make_unique<Bank>();
        bank->set_id(sqlite3_column_int(stmt, 0));
        bank->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        banks.push_back(std::move(bank));
    }

    sqlite3_finalize(stmt);
    return banks;
}

void BankRepository::update(Bank* bank) const {
    std::string sql = std::format("UPDATE banks SET name = '{}' WHERE id = {};", bank->get_name(), std::to_string(bank->get_id()));
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error_message = errMsg;
        sqlite3_free(errMsg);
        throw DatabaseException(error_message);
    }
}
bool BankRepository::exists(const int id) const
{
    std::string sql = "SELECT EXISTS(SELECT 1 FROM banks WHERE id = ?);";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking bank existence");
    }

    sqlite3_bind_int(stmt, 1, id);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return exists;
}