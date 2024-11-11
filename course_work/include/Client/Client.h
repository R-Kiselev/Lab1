#pragma once
#include <string>
#include <iostream>
#include "../interfaces.h"

class Client : public IEntity
{
private:
    int id_;
    std::string name_;
    int social_status_id_;
public:
    Client() = default;
    Client(const std::string& name, const int social_status_id);
    ~Client() override = default;
    int get_id() const override;
    void set_id(const int id) override;
    std::string get_name() const;
    void set_name(const std::string_view& name);
    int get_social_status_id() const;
    void set_social_status_id(const int social_status_id);
    Client(const Client&) = delete;
    void operator=(const Client&) = delete;
};
