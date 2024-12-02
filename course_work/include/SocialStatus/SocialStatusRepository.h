#pragma once

#include "SocialStatus.h"
#include <iostream>
#include <vector>
#include <memory>
#include "../../sqlite/sqlite3.h"
#include "../../include/Errors/CustomExceptions.h"
#include "../interfaces.h"
#include <format>

class SocialStatusRepository : public IRepository<SocialStatus>
{
private:
    sqlite3* db_;
public:
    explicit SocialStatusRepository(sqlite3* db): db_(db){};

    void add(SocialStatus* SocialStatus) const override;
    void remove(const int id) override;
    std::unique_ptr<SocialStatus> get_by_id(const int id) const override;
    list<std::unique_ptr<SocialStatus>> get_all() const override;
    void update(SocialStatus* SocialStatus) const override;
    bool exists(const int id) const override;

    SocialStatusRepository(const SocialStatusRepository&) = delete;
    void operator=(const SocialStatusRepository&) = delete;
};