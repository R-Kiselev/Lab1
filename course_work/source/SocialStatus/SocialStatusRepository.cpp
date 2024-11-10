#include "../../include/SocialStatus/SocialStatusRepository.h"


void SocialStatusRepository::add(SocialStatus* social_status) const {
    std::string sql = "INSERT INTO social_statuses (name) VALUES ('" + social_status->get_name() + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error_message = errMsg;
        sqlite3_free(errMsg);
        throw DatabaseException(error_message);
    }
    int last_id = int(sqlite3_last_insert_rowid(db_));
    social_status->set_id(last_id);
}

void SocialStatusRepository::remove(int id) {
    std::string sql = std::format("DELETE FROM social_statuses WHERE id = {} ;", std::to_string(id));
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error_message = errMsg;
        sqlite3_free(errMsg);
        throw DatabaseException(error_message);
    }
}

std::unique_ptr<SocialStatus> SocialStatusRepository::get_by_id(int id) const {
    std::string sql = std::format("SELECT id, name FROM social_statuses WHERE id = {};",std::to_string(id));
    sqlite3_stmt* stmt;
    auto social_status = std::make_unique<SocialStatus>();

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        social_status->set_id(sqlite3_column_int(stmt, 0));
        social_status->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    } else {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("SocialStatus with ID {} not found", std::to_string(id)));
    }

    sqlite3_finalize(stmt);
    return social_status;
}

std::vector<std::unique_ptr<SocialStatus>> SocialStatusRepository::get_all() const {
    std::vector<std::unique_ptr<SocialStatus>> social_statuses;
    std::string sql = "SELECT id, name FROM social_statuses;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto social_status = std::make_unique<SocialStatus>();
        social_status->set_id(sqlite3_column_int(stmt, 0));
        social_status->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        social_statuses.push_back(std::move(social_status));
    }

    sqlite3_finalize(stmt);
    return social_statuses;
}

void SocialStatusRepository::update(SocialStatus* social_status) const {
    std::string sql = std::format("UPDATE social_statuses SET name = '{}' WHERE id = {};", social_status->get_name(), std::to_string(social_status->get_id()));
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error_message = errMsg;
        sqlite3_free(errMsg);
        throw DatabaseException(error_message);
    }
}
bool SocialStatusRepository::exists(const int id) const
{
    std::string sql = "SELECT EXISTS(SELECT 1 FROM social_statuses WHERE id = ?);";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking social status existence");
    }

    sqlite3_bind_int(stmt, 1, id);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return exists;
}
