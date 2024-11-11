#pragma once

#include <string>
#include <iostream>
#include "../interfaces.h"

class SocialStatus : public IEntity
{
private:
    int id_;
    std::string name_;
protected:
    friend class SocialStatusRepository;
    friend class SocialStatusService;

public:
    explicit SocialStatus() = default;
    ~SocialStatus() override = default;
    explicit SocialStatus(const std::string& name);
    int get_id() const override;
    void set_id(const int id) override;
    std::string get_name() const;
    void set_name(const std::string_view& name);
    SocialStatus(const SocialStatus&) = delete;
    void operator=(const SocialStatus&) = delete;
};