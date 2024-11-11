#pragma once
#include <string>
#include <iostream>
#include "../interfaces.h"

class Bank : public IEntity
{
private:
    int id_;
    std::string name_;
protected:
    friend class BankRepository;
    friend class BankService;

public:
    explicit Bank() = default;
    ~Bank() override = default;
    explicit Bank(const std::string& name);
    void set_id(const int id) override;
    int get_id() const override;
    std::string get_name() const;
    void set_name(const std::string_view& name);
    Bank(const Bank&) = delete;
    void operator=(const Bank&) = delete;
};