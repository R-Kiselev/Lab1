#pragma once
#include "../sqlite/sqlite3.h"
#include <string>
#include <iostream>

void create_table(sqlite3* DB, const std::string& query)
{
    char* errorMessage;
    int rc = sqlite3_exec(DB, query.c_str(), nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

void create_banks_table(sqlite3* DB)
{
    std::string query =
        "CREATE TABLE IF NOT EXISTS banks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL);";

    create_table(DB, query);
}

void create_social_statuses_table(sqlite3* DB)
{
    std::string query =
        "CREATE TABLE IF NOT EXISTS social_statuses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL);";

    create_table(DB, query);
}

void create_clients_table(sqlite3* DB)
{
    std::string query =
        "CREATE TABLE IF NOT EXISTS clients ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL,"
        "name TEXT NOT NULL,"
        "social_status_id INTEGER,"
        "is_admin INTEGER DEFAULT 0 NOT NULL,"
        "FOREIGN KEY (social_status_id) REFERENCES social_statuses(id) ON DELETE CASCADE);";

    create_table(DB, query);
}

void create_cards_table(sqlite3* DB)
{
    std::string query =
        "CREATE TABLE IF NOT EXISTS cards ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "number TEXT NOT NULL UNIQUE,"
        "expire_date TEXT NOT NULL,"
        "balance INTEGER DEFAULT 0 NOT NULL,"
        "account_id INTEGER,"
        "FOREIGN KEY (account_id) REFERENCES accounts(id) ON DELETE CASCADE);";

    create_table(DB, query);
}

void create_accounts_table(sqlite3* DB)
{
    std::string query =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "balance INTEGER DEFAULT 0 NOT NULL,"
        "client_id INTEGER NOT NULL,"
        "bank_id INTEGER NOT NULL,"
        "FOREIGN KEY (client_id) REFERENCES clients(id) ON DELETE CASCADE,"
        "FOREIGN KEY (bank_id) REFERENCES banks(id) ON DELETE CASCADE);";

    create_table(DB, query);
}