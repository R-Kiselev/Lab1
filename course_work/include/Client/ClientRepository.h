#pragma once
#include "Client.h"
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"
#include "../interfaces.h"
#include <format>

class ClientRepository : public IRepository<Client>
{
private:
    sqlite3* db_;
public:
    explicit ClientRepository(sqlite3* db): db_(db){};

    void add(Client* client) const override;
    void remove(const int id) override;
    std::unique_ptr<Client> get_by_id(const int id) const override;
    std::vector<std::unique_ptr<Client>> get_all() const override;
    void update(Client* client) const override;
    bool exists(const int id) const override;

    std::vector<std::unique_ptr<Client>> get_all_by_bank_id(const int bank_id);
    bool has_accounts(const int id) const;
    std::unique_ptr<Client> get_by_username(const std::string& username);

    ClientRepository(const ClientRepository&) = delete;
    void operator=(const ClientRepository&) = delete;
};