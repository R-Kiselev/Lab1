#include "../../include/SocialStatus/SocialStatus.h"
#include <iostream>
#include <string>

SocialStatus::SocialStatus(const std::string& name)
        : name_(name)
{
}
int SocialStatus::get_id() const
{
    return id_;
}
std::string SocialStatus::get_name() const
{
    return name_;
}
void SocialStatus::set_id(const int id) {
    id_ = id;
}
void SocialStatus::set_name(const std::string_view& name)
{
    name_ = name;
}