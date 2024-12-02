#pragma once
#include "../sqlite/sqlite3.h"
#include <vector>
#include <memory>
#include "list.h"

template <typename T>
class IRepository {
public:
    IRepository() = default;
    virtual ~IRepository() = default;

    virtual void add(T* entity) const = 0;
    virtual void remove(const int id) = 0;
    virtual std::unique_ptr<T> get_by_id(const int id) const = 0;
    virtual list<std::unique_ptr<T>> get_all() const = 0;
    virtual void update(T* entity) const = 0;
    virtual bool exists(const int id) const = 0;

    IRepository(const IRepository&) = delete;
    void operator=(const IRepository&) = delete;
};

template <typename T>
class IService {
public:
    IService() = default;
    virtual ~IService() = default;

    virtual void add(T* entity) const = 0;
    virtual void remove(int id) = 0;
    virtual void update(int id, T* entity) const = 0;
    virtual std::unique_ptr<T> get_by_id(int id) const = 0;
    virtual list<std::unique_ptr<T>> get_all() const = 0;
    virtual bool exists(int id) const = 0;

    IService(const IService&) = delete;
    IService& operator=(const IService&) = delete;
};

class IEntity {
public:
    virtual ~IEntity() = default;

    virtual int get_id() const = 0;
    virtual void set_id(int id) = 0;
};

class IFinancial {
public:
    virtual ~IFinancial() = default;

    virtual void set_balance(int balance) = 0;
    virtual int get_balance() const = 0;
};