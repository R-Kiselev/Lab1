#pragma once

#include <iostream>
#include "SocialStatusRepository.h"
#include "../validation_utils.h"
#include "../interfaces.h"

class SocialStatusService :public IService<SocialStatus>
{
private:
    SocialStatusRepository* social_status_repository_;
    std::unique_ptr<ValidationService> validation_service;
public:
    explicit SocialStatusService(SocialStatusRepository* social_status_repository_);
    ~SocialStatusService() override = default;

    void add(SocialStatus* social_status) const override;
    void remove(int id) override;
    void update(int id, SocialStatus* new_social_status) const override;
    std::unique_ptr<SocialStatus> get_by_id(int id) const override;
    list<std::unique_ptr<SocialStatus>>get_all() const override;
    bool exists(const int id) const override;
    void display_all() const;

    SocialStatusService(const SocialStatusService&) = delete;
    void operator=(const SocialStatusService&) = delete;
};