#pragma once
#include <iostream>
#include "BankRepository.h"
#include "../interfaces.h"
#include "../validation_utils.h"

class BankService : public IService<Bank>{
private:
    BankRepository* bank_repository_;
    std::unique_ptr<ValidationService> validation_service;
public:
    explicit BankService(BankRepository* bank_repository);

    void add(Bank* bank) const override;
    void remove(int id) override;
    void update(int id, Bank* bank) const override;
    std::unique_ptr<Bank> get_by_id(int id) const override;
    list<std::unique_ptr<Bank>>get_all() const override;
    bool exists(const int id) const override;

    void display_all() const;

    BankService(const BankService&) = delete;
    void operator=(const BankService&) = delete;
};