#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"
#include "../interfaces.h"

class Account : public IEntity, public IFinancial
{
private:
    int id_;
    int balance_ = 0;
    int client_id_;
    int bank_id_;
protected:
    friend class AccountRepository;
    friend class AccountService;

public:
    explicit Account(const int client_id = 0, const int bank_id = 0);
    ~Account() override = default;

    void set_id(const int id) override;
    int get_id() const override;
    void set_balance(const int balance) override;
    int get_balance() const override;
    void set_client_id(const int id);
    int get_client_id() const;
    void set_bank_id(const int id);
    int get_bank_id() const;


    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};
