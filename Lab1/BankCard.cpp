#include "BankCard.h"

BankCard::BankCard(const std::string& card_number, const std::string& expire_date, const int card_balance)
    : card_number_(card_number), expire_date_(expire_date), card_balance_(card_balance)
{
}

BankCard::~BankCard()
{
    std::cout << "Card deleted: " << card_number_ << std::endl;
}

void BankCard::display_card_info() const
{
    std::cout << "Card number: " << card_number_ << std::endl;
    std::cout << "Expire date: " << expire_date_ << std::endl;
    std::cout << "Card balance: " << card_balance_ << std::endl;
}

void BankCard::set_card_number(const std::string_view& card_number)
{
    card_number_ = std::string(card_number);
}

void BankCard::set_expire_date(const std::string_view& expire_date)
{
    expire_date_ = std::string(expire_date);
}

void BankCard::set_card_balance(const int card_balance)
{
    card_balance_ = card_balance;
}

std::string BankCard::get_card_number() const
{
    return card_number_;
}

std::string BankCard::get_expire_date() const
{
    return expire_date_;
}

int BankCard::get_card_balance() const
{
    return card_balance_;
}
