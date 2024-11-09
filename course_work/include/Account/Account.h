#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"

class Account
{
private:
    int id_;
    int balance_;
    int client_id_;
    int bank_id_;
protected:
    friend class AccountRepository;
    friend class AccountService;

public:
    Account(const int client_id = 0, const int bank_id = 0);
    ~Account() = default;

    void set_id(const int id);
    int get_id() const;
    void set_balance(const int balance);
    int get_balance() const;
    void set_client_id(const int id);
    int get_client_id() const;
    void set_bank_id(const int id);
    int get_bank_id() const;


    Account(const Account&) = delete;
    void operator=(const Account&) = delete;
};
