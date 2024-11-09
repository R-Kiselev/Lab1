#include "../../include/Client/Client.h"
#include <iostream>
#include <string>

Client::Client(const std::string& name, const int social_status_id)
    : name_(name), social_status_id_(social_status_id)
{
}
void Client::set_id(const int id) {
    id_ = id;
}
int Client::get_id() const
{
    return id_;
}
void Client::set_name(const std::string& name)
{
    name_ = name;
}
std::string Client::get_name() const
{
    return name_;
}
void Client::set_social_status_id(const int social_status_id)
{
    social_status_id_ = social_status_id;
}
int Client::get_social_status_id() const
{
    return social_status_id_;
}