#pragma once
#include <optional>
#include "CardRepository.h"
#include "../Account/AccountService.h"

class CardService {
private:
    CardRepository* card_repository_;
    AccountService* account_service_;
public:
    CardService(CardRepository* card_repository, AccountService* account_service);
    ~CardService() = default;

    void add(const int account_id) const;
    void remove(int id);
    void update(int id, int balance) const;
    std::unique_ptr<Card> get_by_id(int id) const;
    std::unique_ptr<Card> get_card_by_number(std::string& number) const;
    std::vector<std::unique_ptr<Card>> get_all() const;
    std::vector<std::unique_ptr<Card>> get_cards_by_account_id(const int account_id) const;
    void display_all_cards() const;
    bool exists (const int id) const;

    CardService(const CardService&) = delete;
    void operator=(const CardService&) = delete;

    friend bool compare_balances(const Account& account1, const Account& account2);
};