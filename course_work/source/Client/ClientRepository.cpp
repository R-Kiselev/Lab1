#include "../../include/Client/ClientRepository.h"

void ClientRepository::add(Client *client) const
{
    std::string sql = "INSERT INTO clients (name, social_status_id) VALUES (?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for adding client");
    }
    sqlite3_bind_text(stmt, 1, client->get_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, client->get_social_status_id());
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to insert client into database");
    }
    int last_id = int(sqlite3_last_insert_rowid(db_));
    client->set_id(last_id);
    sqlite3_finalize(stmt);
}
void ClientRepository::remove(const int id)
{
    std::string sql = "DELETE FROM clients WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for removing client");
    }
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to remove client from database");
    }
    sqlite3_finalize(stmt);
}
std::unique_ptr<Client> ClientRepository::get_by_id(const int id) const
{
    std::string sql = "SELECT id, name, social_status_id FROM clients WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by ID");
    }
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException("Client with ID " + std::to_string(id) + " not found in database");
    }
    int client_id = sqlite3_column_int(stmt, 0);
    std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    int social_status_id = sqlite3_column_int(stmt, 2);

    auto client = std::make_unique<Client>(name, social_status_id);
    client->set_id(client_id);

    return client;
}
std::vector<std::unique_ptr<Client>> ClientRepository::get_all() const
{
    std::vector<std::unique_ptr<Client>> clients;
    std::string sql = "SELECT id, name, social_status_id FROM clients;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by ID");
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        auto client = std::make_unique<Client>();
        client->set_id(sqlite3_column_int(stmt, 0));
        client->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        client->set_social_status_id(sqlite3_column_int(stmt, 2));
        clients.push_back(std::move(client));
    }
    sqlite3_finalize(stmt);
    if (clients.empty()) {
        throw NotFoundException("No clients found in database");
    }
    return clients;
}
void ClientRepository::update(Client* client) const
{
    std::string sql = "UPDATE clients SET name = ?, social_status_id = ? WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for updating client");
    }
    sqlite3_bind_text(stmt, 1, client->get_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, client->get_social_status_id());
    sqlite3_bind_int(stmt, 3, client->get_id());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to update client in database");
    }

    sqlite3_finalize(stmt);
}
bool ClientRepository::exists(const int id) const
{
    std::string sql = "SELECT EXISTS(SELECT 1 FROM clients WHERE id = ?);";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking client existence");
    }

    sqlite3_bind_int(stmt, 1, id);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return exists;
}
std::vector<std::unique_ptr<Client>> ClientRepository::get_all_by_bank_id(const int bank_id)
{
    std::string sql =
            "SELECT clients.id, clients.name, clients.social_status_id "
            "FROM clients "
            "JOIN accounts ON clients.id = accounts.client_id "
            "WHERE accounts.bank_id = ?"
            "GROUP BY clients.id;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting all clients by bank ID");
    }
    sqlite3_bind_int(stmt, 1, bank_id);
    std::vector<std::unique_ptr<Client>> clients;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto client = std::make_unique<Client>();
        client->set_id(sqlite3_column_int(stmt, 0));
        client->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        client->set_social_status_id(sqlite3_column_int(stmt, 2));
        clients.push_back(std::move(client));
    }
    sqlite3_finalize(stmt);
    if (clients.empty()) {
        throw NotFoundException("No clients found in database");
    }
    return clients;
}
bool ClientRepository::has_accounts(int id) const {
    sqlite3_stmt* stmt;
    const char* query = "SELECT COUNT(*) FROM accounts WHERE client_id = ?;";

    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw CustomException("Failed to prepare statement: " + std::string(sqlite3_errmsg(db_)));
    }

    sqlite3_bind_int(stmt, 1, id);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}