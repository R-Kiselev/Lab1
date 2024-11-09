#pragma once
#include "../sqlite/sqlite3.h"
#include <vector>
#include <memory>

template <typename T>
class IRepository {
private:
    sqlite3* db_;
public:
    IRepository() = default;
    virtual ~IRepository() = default;

    virtual void add(T* entity) const = 0;
    virtual void remove(const int id) = 0;
    virtual std::unique_ptr<T> get_by_id(const int id) const = 0;
    virtual std::vector<std::unique_ptr<T>> get_all() const = 0;
    virtual void update(T* entity) const = 0;
    virtual bool exists(const int id) const = 0;

    IRepository(const IRepository&) = delete;
    void operator=(const IRepository&) = delete;
};