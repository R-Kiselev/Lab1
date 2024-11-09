#pragma once

#include <string>
#include <iostream>

class SocialStatus{
private:
    int id_;
    std::string name_;
protected:
    friend class SocialStatusRepository;
    friend class SocialStatusService;

    void set_id(const int id);
public:
    explicit SocialStatus() = default;
    ~SocialStatus() = default;
    SocialStatus(const std::string& name);
    int get_id() const;
    std::string get_name() const;
    void set_name(const std::string_view& name);
    SocialStatus(const SocialStatus&) = delete;
    void operator=(const SocialStatus&) = delete;
};