#include "../../include/SocialStatus/SocialStatus.h"
#include <iostream>
#include <string>

SocialStatus::SocialStatus()
{
}
SocialStatus::SocialStatus(const std::string& name)
        : name_(name)
{
}
SocialStatus::~SocialStatus()
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
void SocialStatus::set_name(const std::string& name)
{
    name_ = name;
}