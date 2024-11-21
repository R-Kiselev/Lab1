#include "../../include/Client/Client.h"
#include <iostream>
#include <string>

Client::Client(const std::string& name, const int social_status_id)
    : name_(name), social_status_id_(social_status_id)
{
}
Client::Client(const std::string& username, const std::string& password, const std::string& name, const int social_status_id)
    :  username_(username), password_(password), name_(name), social_status_id_(social_status_id)
{
}
void Client::set_id(const int id) {
    id_ = id;
}
int Client::get_id() const
{
    return id_;
}
void Client::set_username(const std::string_view& username)
{
    username_ = username;
}
std::string Client::get_username() const
{
    return username_;
}
void Client::set_password(const std::string_view& password)
{
    password_ = password;
}
std::string Client::get_password() const
{
    return password_;
}
void Client::set_name(const std::string_view& name)
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
void Client::set_is_admin(bool is_admin)
{
    is_admin_ = is_admin;
}
bool Client::get_is_admin() const
{
    return is_admin_;
}