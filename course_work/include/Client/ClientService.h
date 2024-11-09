#pragma once
#include <iostream>
#include <optional>
#include "ClientRepository.h"
#include "../SocialStatus/SocialStatusService.h"

class ClientService
{
private:
    ClientRepository* client_repository_;
    SocialStatusService* social_status_service_;
public:
    ClientService(ClientRepository* client_repository_, SocialStatusService* social_status_service_);
    ~ClientService() = default;

    std::unique_ptr<Client> add(const std::string& name, const int social_status_id) const;
    void remove(int id);
    void update(int id,const std::optional<std::string>& name, const std::optional<int>& social_status_id);
    std::unique_ptr<Client> get_by_id(int id) const;
    std::vector<std::unique_ptr<Client>> get_all() const;
    void display_all() const;
    bool exists(const int id) const;
    std::vector<std::unique_ptr<Client>> get_all_by_bank_id(const int bank_id);
    bool has_accounts(const int id) const;

    ClientService(const ClientService&) = delete;
    void operator=(const ClientService&) = delete;
};