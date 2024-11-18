#pragma once
#include <iostream>
#include <optional>
#include "ClientRepository.h"
#include "../SocialStatus/SocialStatusService.h"
#include "../validation_utils.h"
#include "../interfaces.h"

class ClientService : public IService<Client>
{
private:
    ClientRepository* client_repository_;
    SocialStatusService* social_status_service_;
    std::unique_ptr<ValidationService> validation_service;
public:
    explicit ClientService(ClientRepository* client_repository_, SocialStatusService* social_status_service_);
    ~ClientService() override = default;

    void add(Client* client) const override;
    void remove(int id) override;
    void update(int id, Client* client) const override;
    std::unique_ptr<Client> get_by_id(int id) const override;
    std::vector<std::unique_ptr<Client>> get_all() const override;
    bool exists(const int id) const override;

    void display_all() const;

    std::vector<std::unique_ptr<Client>> get_all_by_bank_id(const int bank_id);
    std::vector<std::unique_ptr<Client>> get_all_by_account_id(const int account_id);

    bool has_accounts(const int id) const;

    ClientService(const ClientService&) = delete;
    void operator=(const ClientService&) = delete;
};