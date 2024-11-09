#include "Account.h"
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"
#include "../interfaces.h"
#include <format>

class AccountRepository : public IRepository<Account>
{
private:
    sqlite3* db_;
public:
    explicit AccountRepository(sqlite3* db): db_(db){};

    void add(Account* account) const override;
    std::unique_ptr<Account> get_by_id(const int id) const override;
    std::vector<std::unique_ptr<Account>> get_all() const override;
    void update(Account* account) const override;
    void remove(const int id) override;
    bool exists(const int id) const override;

    std::vector<std::unique_ptr<Account>> get_all_by_client_id(int client_id) const;

    AccountRepository(const AccountRepository&) = delete;
    void operator=(const AccountRepository&) = delete;
};