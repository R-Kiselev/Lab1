#include <iostream>
#include <vector>
#include <string>

class BankCard
{
private:
    std::string card_number_;
    std::string expire_date_;
    int card_balance_;
public:
    BankCard(const std::string& card_number, const std::string& expire_date, const int card_balance = 0)
        : card_number_(card_number), expire_date_(expire_date), card_balance_(card_balance)
    {
    }

    void display_card_info() const
    {
        std::cout << "Card number: " << card_number_ << std::endl;
        std::cout << "Expire date: " << expire_date_ << std::endl;
        std::cout << "Card balance: " << card_balance_ << std::endl;
    }

    void set_card_number(const std::string& card_number)
    {
        card_number_ = card_number;
    }

    void set_expire_date(const std::string& expire_date)
    {
        expire_date_ = expire_date;
    }

    void set_card_balance(const int card_balance)
    {
        card_balance_ = card_balance;
    }

    std::string get_card_number() const
    {
        return card_number_;
    }

    std::string get_expire_date() const
    {
        return expire_date_;
    }

    int get_card_balance() const
    {
        return card_balance_;
    }
};

class Account
{
private:
    std::string client_name_;
    std::vector<BankCard*> cards_;
    int account_balance_;
public:
    Account(const std::string& client_name, const int account_balance)
        : client_name_(client_name), account_balance_(account_balance)
    {
    }

    ~Account()
    {
        for (BankCard* card : cards_)
        {
            delete card;
        }
        cards_.clear();
    }

    void display_account_info() const
    {
        std::cout << "Client name: " << client_name_;
        std::cout << "Account balance: " << account_balance_;
        
        if (!cards_.empty())
        {
            std::cout << "Attached cards: " << std::endl << std::endl;
            for (BankCard* card : cards_)
            {
                card->display_card_info();
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "No cards are attached to this account." << std::endl;
        }
    }

    void add_bank_card(BankCard* card)
    {
        cards_.push_back(card);
    }

    void delete_bank_card(const std::string card_number)
    {
        for (std::vector<BankCard*>::iterator iter = cards_.begin(); iter != cards_.end(); iter++)
        {
            if ((*iter)->get_card_number() == card_number)
            {
                cards_.erase(iter);
                delete *iter;
                break;
            }
        }
    }

    int get_account_available_balance() const
    {
        int cards_balance = 0;
        int balance_available = 0;

        for (BankCard* card : cards_)
        {
            cards_balance += card->get_card_balance();
        }

        if (account_balance_ - cards_balance > 0)
        {
            balance_available = account_balance_ - cards_balance;
        }

        return balance_available;
    }

};

int main()
{

}