#include "BankCard.h"

static bool is_card_number(const std::string_view& card_number)
{
    std::vector<char> accepted_symbols = { '-', ' ' };

    for (char ch : card_number)
    {
        if (!std::isdigit(ch) && std::ranges::find(accepted_symbols.begin(), accepted_symbols.end(), ch) == accepted_symbols.end())
        {
            return false;
        }
    }
    return true;
}

static bool is_expire_date(const std::string_view& exp_date)
{
    std::vector<char> accepted_symbols = { '-', '\\', '/'};
    int symb_count = 0;
    int digits_count = 0;
    for (char ch : exp_date)
    {
        if (!std::isdigit(ch))
        {
            if (ch == '-')
            {
                symb_count++;
                if (symb_count > 1) return false;
            }
            else if (std::find(accepted_symbols.begin(), accepted_symbols.end(), ch) == accepted_symbols.end())
            {
                return false;
            }
        }
        else
            digits_count++;
    }
    if (digits_count != 4)
        return false;
    return true;
}
Card::Card(const int id,
    const std::string& card_number,
    const std::string& expire_date,
    const int balance,
    const int account_id)
    : id_(id), balance_(balance), account_id_(account_id)
{
    if (is_card_number(card_number) && is_expire_date(expire_date))
    {
        number_ = card_number;
        expire_date_ = expire_date;
    }
    else
    {
        throw std::runtime_error("Wrong symbols in card number or expire date.");
    }
}

Card::~Card()
{
    std::cout << "Card ID: " << get_id() << std::endl;
}

void Card::display_info() const
{
    std::cout << "Id: " << id_ << std::endl;
    std::cout << "Card number: " << number_ << std::endl;
    std::cout << "Expire date: " << expire_date_ << std::endl;
    std::cout << "Card balance: " << balance_ << std::endl;
}

void Card::set_number(const std::string_view& card_number)
{
    if (is_card_number(card_number))
    {
        number_ = std::string(card_number);
    }
    else
    {
        std::cerr << "Wrong symbols in card number." << std::endl << std::endl;
    }
}
void Card::set_expire_date(const std::string_view& expire_date)
{
    if (is_card_number(expire_date))
    {
        expire_date_ = std::string(expire_date);
    }
    else
    {
        std::cerr << "Wrong symbols in expire date." << std::endl << std::endl;
    }
}
void Card::set_balance(const int card_balance)
{
    balance_ = card_balance;
}

void Card::set_id(const int id)
{
    id_ = id;
}

void Card::set_account_id(const int id)
{
    account_id_ = id;
}


std::string Card::get_number() const
{
    return number_;
}
std::string Card::get_expire_date() const
{
    return expire_date_;
}
int Card::get_balance() const
{
    return balance_;
}

int Card::get_id() const
{
    return id_;
}

int Card::get_account_id() const
{
    return account_id_;
}