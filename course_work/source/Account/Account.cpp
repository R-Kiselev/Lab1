#include "../../include/Account/Account.h"

Account::Account(const int client_id, const int bank_id):
    client_id_(client_id), bank_id_(bank_id)
{
}
void Account::set_id(const int id)
{
    id_ = id;
}
int Account::get_id() const
{
    return id_;
}
void Account::set_balance(const int account_balance)
{
    balance_ = account_balance;
}
int Account::get_balance() const
{
    return balance_;
}
void Account::set_client_id(const int client_id) {
    client_id_ = client_id;
}
int Account::get_client_id() const {
    return client_id_;
}
void Account::set_bank_id(const int bank_id) {
    bank_id_ = bank_id;
}
int Account::get_bank_id() const {
    return bank_id_;
}