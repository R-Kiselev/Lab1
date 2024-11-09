#include <sqlite3.h>
#include <vector>
#include <memory>

class IDatabaseObject abstract
{
public:
    virtual void save_to_db(sqlite3* db) const = 0;
    virtual void delete_from_db(sqlite3* db) = 0;
};

class IDatabaseCollection abstract
{
public:
    virtual void save_collection_to_db(sqlite3* db) const = 0;
    virtual void load_collection_from_db(sqlite3* db) = 0;
};

class IFinancial abstract
{
public:
    virtual int get_balance() const = 0;
    virtual void set_balance(const int balance) = 0;
};