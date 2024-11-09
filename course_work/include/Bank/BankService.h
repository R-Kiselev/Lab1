#pragma once
#include <iostream>
#include "BankRepository.h"

class BankService{
private:
    BankRepository* bank_repository_;
public:
    BankService(BankRepository* bank_repository);
    ~BankService() = default;

    void add(const std::string& name) const;
    void remove(int id);
    void update(int id, const std::string& name);
    std::unique_ptr<Bank> get_by_id(int id) const;
    std::vector<std::unique_ptr<Bank>>get_all() const;
    void display_all() const;
    bool exists(const int id) const;

    BankService(const BankService&) = delete;
    void operator=(const BankService&) = delete;
};