#pragma once
#include <string>
#include <iostream>
#include "../interfaces.h"

class Client : public IEntity
{
private:
    int id_;
    std::string username_;
    std::string password_;
    std::string name_;
    int social_status_id_;
    bool is_admin_ = false;
public:
    Client() = default;
    Client(const std::string& name, const int social_status_id);
    Client(const std::string& username, const std::string& password, const std::string& name, const int social_status_id);
    ~Client() override = default;
    int get_id() const override;
    void set_id(const int id) override;
    void set_username(const std::string_view& username);
    std::string get_username() const;
    void set_password(const std::string_view& password);
    std::string get_password() const;
    std::string get_name() const;
    void set_name(const std::string_view& name);
    int get_social_status_id() const;
    void set_social_status_id(const int social_status_id);
    void set_is_admin(bool is_admin);
    bool get_is_admin() const;
    Client(const Client&) = delete;
    void operator=(const Client&) = delete;
};
