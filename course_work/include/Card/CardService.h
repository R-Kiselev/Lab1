#pragma once
#include <optional>
#include "CardRepository.h"
#include "../Account/AccountService.h"
#include "../validation_utils.h"
#include "../interfaces.h"

class CardService : public IService<Card>{
private:
    CardRepository* card_repository_;
    AccountService* account_service_;
    std::unique_ptr<ValidationService> validation_service;
public:
    CardService(CardRepository* card_repository, AccountService* account_service);
    ~CardService() override = default;

    void add(Card* card) const override;
    void remove(int id) override;
    void update(int id, Card* card) const override;
    std::unique_ptr<Card> get_by_id(int id) const override;
    std::vector<std::unique_ptr<Card>> get_all() const override;
    bool exists (const int id) const override;

    bool exists(std::string number) const;
    std::unique_ptr<Card> get_by_number(std::string& number) const;
    std::vector<std::unique_ptr<Card>> get_all_by_client_id(const int client_id) const;
    std::vector<std::unique_ptr<Card>> get_all_by_account_id(const int account_id) const;
    void display_all_cards() const;

    CardService(const CardService&) = delete;
    void operator=(const CardService&) = delete;

    friend bool compare_balances(const Account& account1, const Account& account2);
};