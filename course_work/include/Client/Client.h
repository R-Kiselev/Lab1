#pragma once
#include <string>
#include <iostream>

class Client
{
private:
    int id_;
    std::string name_;
    int social_status_id_;
public:
    Client() = default;
    Client(const std::string& name, const int social_status_id);
    ~Client() = default;
    int get_id() const;
    void set_id(const int id);
    std::string get_name() const;
    void set_name(const std::string& name);
    int get_social_status_id() const;
    void set_social_status_id(const int social_status_id);
    Client(const Client&) = delete;
    void operator=(const Client&) = delete;
};
