#include "../../include/Client/ClientRepository.h"

void ClientRepository::add(Client *client) const
{
    std::string sql = "INSERT INTO clients (name, social_status_id, is_admin, username, password) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for adding client");
    }
    sqlite3_bind_text(stmt, 1, client->get_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, client->get_social_status_id());
    sqlite3_bind_int(stmt, 3, client->get_is_admin());
    sqlite3_bind_text(stmt, 4, client->get_username().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, client->get_password().c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to insert client into database");
    }
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
    std::string sql = "SELECT id, name, social_status_id, is_admin FROM clients WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by ID");
    }
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Client with ID {} not found in database", std::to_string(id)));
    }
    int client_id = sqlite3_column_int(stmt, 0);
    std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    int social_status_id = sqlite3_column_int(stmt, 2);
    bool is_admin = sqlite3_column_int(stmt, 3) == 1;

    auto client = std::make_unique<Client>(name, social_status_id);
    client->set_is_admin(is_admin);
    client->set_id(client_id);

    return client;
}
std::vector<std::unique_ptr<Client>> ClientRepository::get_all() const
{
    std::vector<std::unique_ptr<Client>> clients;
    std::string sql = "SELECT id, name, social_status_id, username, password, is_admin FROM clients;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by ID");
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        int client_id = sqlite3_column_int(stmt, 0);
        std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        int social_status_id = sqlite3_column_int(stmt, 2);
        bool is_admin = sqlite3_column_int(stmt, 3) == 1;
        std::string username(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        std::string password(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        auto client = std::make_unique<Client>(username, password, name, social_status_id);
        client->set_is_admin(is_admin);
        client->set_id(client_id);

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
            "SELECT clients.id, clients.name, clients.social_status_id, clients.username, clients.is_admin "
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
        int client_id = sqlite3_column_int(stmt, 0);
        std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        int social_status_id = sqlite3_column_int(stmt, 2);
        bool is_admin = sqlite3_column_int(stmt, 3) == 1;
        auto client = std::make_unique<Client>(name, social_status_id);
        if (is_admin) {
            client->set_is_admin(true);
        }
        client->set_id(client_id);

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
std::unique_ptr<Client> ClientRepository::get_by_username(const std::string &username) {
    std::string sql = "SELECT id, name, social_status_id, username, password, is_admin FROM clients WHERE username = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by username");
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Client with username {} not found in database", username));
    }
    int client_id = sqlite3_column_int(stmt, 0);
    std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    int social_status_id = sqlite3_column_int(stmt, 2);
    std::string password(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    bool is_admin = sqlite3_column_int(stmt, 5) == 1;

    auto client = std::make_unique<Client>(username, password, name, social_status_id);
    client->set_is_admin(is_admin);
    client->set_id(client_id);

    return client;
}