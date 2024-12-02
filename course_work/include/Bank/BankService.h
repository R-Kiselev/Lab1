#pragma once
#include <iostream>
#include "BankRepository.h"
#include "../interfaces.h"
#include "../validation_utils.h"

class BankService {
private:
    BankRepository* bank_repository_;
    std::unique_ptr<ValidationService> validation_service;
public:
    explicit BankService(BankRepository* bank_repository);

    void add(Bank* bank) const ;
    void remove(int id) ;
    void update(int id, Bank* bank) const ;
    std::unique_ptr<Bank> get_by_id(int id) const ;
    list<std::unique_ptr<Bank>>get_all() const ;
    bool exists(const int id) const ;

    void display_all() const;

    BankService(const BankService&) = delete;
    void operator=(const BankService&) = delete;
};