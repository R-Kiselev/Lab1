#include "../../include/Bank/Bank.h"
#include <iostream>
#include <string>

Bank::Bank()
{
}
Bank::Bank(const std::string& name)
    : name_(name)
{
}
Bank::~Bank()
{
}
int Bank::get_id() const
{
    return id_;
}
std::string Bank::get_name() const
{
    return name_;
}
void Bank::set_id(const int id) {
    id_ = id;
}
void Bank::set_name(const std::string& name)
{
    name_ = name;
}