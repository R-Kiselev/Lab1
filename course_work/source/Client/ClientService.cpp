#include "../../include/Client/ClientService.h"

ClientService::ClientService(ClientRepository* client_repository_,
                             SocialStatusService* social_status_service_)
    : client_repository_(client_repository_),
      social_status_service_(social_status_service_){}
std::unique_ptr<Client> ClientService::add(const std::string& name, const int social_status_id) const
{
    // improve validation check
    if (name.empty()) {
        throw ValidationException("Client name cannot be empty");
    }
    if(!social_status_service_->exists(social_status_id)){
        throw ValidationException("Social status does not exist");
    }
    auto client = std::make_unique<Client>(name, social_status_id);
    client_repository_->add(client.get());
    return client;
}
void ClientService::remove(int id)
{
    auto client = client_repository_->get_by_id(id);
    client_repository_->remove(client->get_id());
}
void ClientService::update(int id,const std::optional<std::string>& name, const std::optional<int>& social_status_id) const
{
    if (name.has_value() && name->empty()) {
        throw ValidationException("New client name cannot be empty");
    }
    if (social_status_id.has_value() && !social_status_service_->exists(*social_status_id)) {
        throw ValidationException("Social status does not exist");
    }

    auto client = client_repository_->get_by_id(id);
    if (name.has_value()) {
        client->set_name(*name);
    }
    if (social_status_id.has_value()) {
        client->set_social_status_id(*social_status_id);
    }
    client_repository_->update(client.get());
}
std::unique_ptr<Client> ClientService::get_by_id(int id) const
{
    return client_repository_->get_by_id(id);
}
std::vector<std::unique_ptr<Client>> ClientService::get_all() const
{
    return client_repository_->get_all();
}
void ClientService::display_all() const
{
    auto clients = client_repository_->get_all();
    if (clients.empty()) {
        std::cout << "No clients available." << std::endl;
    } else {
        for (const auto& client : clients) {
            std::cout
            << "Client ID: " << client->get_id()
            << ", Name: " << client->get_name()
            << ", SocialStatus ID: " << client->get_social_status_id()
            << std::endl;
        }
        std::cout << std::endl;
    }
}
bool ClientService::exists(const int id) const {
    return client_repository_->exists(id);
}
std::vector<std::unique_ptr<Client>> ClientService::get_all_by_bank_id(const int bank_id)
{
    return client_repository_->get_all_by_bank_id(bank_id);
}
bool ClientService::has_accounts(const int id) const
{
    return client_repository_->has_accounts(id);
}