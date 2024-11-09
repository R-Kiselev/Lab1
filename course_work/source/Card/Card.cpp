#include "../../include/Card/Card.h"

Card::Card(const std::string &number, const std::string &expire_date, const int balance, const int account_id)
    : number_(number), expire_date_(expire_date), balance_(balance), account_id_(account_id)
{
}

void Card::set_id(const int id) {
    id_ = id;
}
int Card::get_id() const {
    return id_;
}
void Card::set_number(std::string card_number) {
    number_ = card_number;
}
std::string Card::get_number() const {
    return number_;
}
void Card::set_expire_date(std::string expire_date) {
    expire_date_ = expire_date;
}
std::string Card::get_expire_date() const {
    return expire_date_;
}
void Card::set_balance(const int balance) {
    balance_=balance;
}
int Card::get_balance() const {
    return balance_;
}
void Card::set_account_id(const int account_id) {
    account_id_=account_id;
}
int Card::get_account_id() const {
    return account_id_;
}