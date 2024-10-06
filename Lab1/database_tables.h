#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>


void create_card_table(sqlite3* DB)
{
    char* errorMessage;
    std::string query =
        "CREATE TABLE IF NOT EXISTS cards ("
        "id INT PRIMARY KEY,"
        "card_number TEXT NOT NULL,"
        "expire_date TEXT NOT NULL,"
        "balance INTEGER DEFAULT 0 NOT NULL,"
        "account_id INT,"
        "FOREIGN KEY (account_id) REFERENCES accounts(id) ON DELETE CASCADE);";

    int rc = sqlite3_exec(DB, query.c_str(), NULL, 0, &errorMessage);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating card table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

void create_account_table(sqlite3* DB)
{
    char* errorMessage;
    std::string query =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "id INT RIMARY KEY,"
        "client_name TEXT NOT NULL,"
        "card_balance INTEGER DEFAULT 0 NOT NULL);";

    int rc = sqlite3_exec(DB, query.c_str(), NULL, 0, &errorMessage);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating account table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}