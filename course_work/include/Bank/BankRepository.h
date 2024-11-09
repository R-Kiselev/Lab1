#pragma once
#include "Bank.h"
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"
#include "../interfaces.h"

class BankRepository : public IRepository<Bank> {
private:
    sqlite3* db_;
public:
    explicit BankRepository(sqlite3* db): db_(db){};

    void add(Bank* bank) const override;
    void remove(const int id) override;
    std::unique_ptr<Bank> get_by_id(const int id) const override;
    std::vector<std::unique_ptr<Bank>> get_all() const override;
    void update(Bank* bank) const override;
    bool exists(const int id) const override;

    BankRepository(const BankRepository&) = delete;
    void operator=(const BankRepository&) = delete;
};