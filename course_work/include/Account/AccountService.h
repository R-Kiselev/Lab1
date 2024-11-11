#pragma once
#include <iostream>
#include <optional>
#include "AccountRepository.h"
#include "../Client/ClientService.h"
#include "../Bank/BankService.h"
#include "../validation_utils.h"
#include "../interfaces.h"

class AccountService : public IService<Account>
{
private:
    AccountRepository* account_repository_;
    ClientService* client_service_;
    BankService* bank_service_;
    std::unique_ptr<ValidationService> validation_service;
public:
    AccountService(AccountRepository* account_repository,
                   ClientService* client_service,
                   BankService* bank_service);
    ~AccountService() = default;

    void add(Account* account) const override;
    void remove(int id) override;
    void update(int id, Account* account) const override;
    std::unique_ptr<Account> get_by_id(int id) const override;
    std::vector<std::unique_ptr<Account>> get_all() const override;
    bool exists(const int id) const override;

    void display_all() const;
    [[nodiscard]] std::vector<std::unique_ptr<Account>> get_all_by_client_id(int client_id) const;

    AccountService(const AccountService&) = delete;
    void operator=(const AccountService&) = delete;
};