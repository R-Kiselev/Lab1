#pragma once
#include "Card.h"
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"
#include "../interfaces.h"
#include <format>

class CardRepository : public IRepository<Card>
{
private:
    sqlite3* db_;
public:
    explicit CardRepository(sqlite3* db): db_(db){};
    ~CardRepository() override = default;

    void add(Card* card) const override;
    std::unique_ptr<Card> get_by_id(const int id) const override;
    list<std::unique_ptr<Card>> get_all() const override;
    void update(Card* card) const override;
    void remove(const int id) override;
    bool exists(const int id) const override;

    list<std::unique_ptr<Card>> get_all_by_account_id(const int account_id) const;
    std::vector<std::unique_ptr<Card>> get_all_by_client_id(int client_id) const;
    std::unique_ptr<Card> get_by_number(std::string& number_) const;
    bool exists(std::string number) const;

    CardRepository(const CardRepository&) = delete;
    void operator=(const CardRepository&) = delete;
};