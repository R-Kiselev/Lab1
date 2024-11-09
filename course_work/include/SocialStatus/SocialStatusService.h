#pragma once

#include <iostream>
#include "SocialStatusRepository.h"

class SocialStatusService{
private:
    SocialStatusRepository* social_status_repository_;
public:
    SocialStatusService(SocialStatusRepository* social_status_repository_);
    ~SocialStatusService() = default;

    void add(const std::string& name) const;
    void remove(int id);
    void update(int id, const std::string& name);
    std::unique_ptr<SocialStatus> get_by_id(int id) const;
    std::vector<std::unique_ptr<SocialStatus>>get_all() const;
    void display_all() const;
    bool exists(const int id) const;

    SocialStatusService(const SocialStatusService&) = delete;
    void operator=(const SocialStatusService&) = delete;
};