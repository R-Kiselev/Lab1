#pragma once
#include <string>
#include <iostream>

class Bank{
private:
    int id_;
    std::string name_;
protected:
    friend class BankRepository;
    friend class BankService;

    void set_id(const int id);
public:
    Bank();
    Bank(const std::string& name);
    ~Bank();
    int get_id() const;
    std::string get_name() const;
    void set_name(const std::string& name);
    Bank(const Bank&) = delete;
    void operator=(const Bank&) = delete;
};