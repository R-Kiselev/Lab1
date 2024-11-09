#pragma once
#include <iostream>
#include <optional>
#include "AccountRepository.h"
#include "../Client/ClientService.h"
#include "../Bank/BankService.h"
class AccountService
{
private:
    AccountRepository* account_repository_;
    ClientService* client_service_;
    BankService* bank_service_;
public:
    AccountService(AccountRepository* account_repository,
                   ClientService* client_service,
                   BankService* bank_service);
    ~AccountService() = default;

    void add(const int client_id, const int bank_id) const;
    void remove(int id);
    void update(int id, const int balance) const;
    std::unique_ptr<Account> get_by_id(int id) const;
    std::vector<std::unique_ptr<Account>> get_all() const;
    void display_all() const;
    bool exists(const int id) const;
    [[nodiscard]] std::vector<std::unique_ptr<Account>> get_all_by_client_id(int client_id) const;

    AccountService(const AccountService&) = delete;
    void operator=(const AccountService&) = delete;
};