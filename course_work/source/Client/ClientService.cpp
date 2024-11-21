#include "../../include/Client/ClientService.h"

ClientService::ClientService(ClientRepository* client_repository_,
                             SocialStatusService* social_status_service_)
    : client_repository_(client_repository_),
      social_status_service_(social_status_service_)
{
    validation_service = std::make_unique<ValidationService>();
}
void ClientService::add(Client* client) const
{
    validation_service->validate_name(client->get_name());
    if(!social_status_service_->exists(client->get_social_status_id())){
        throw ValidationException("Social status does not exist");
    }
    client_repository_->add(client);
}
void ClientService::remove(int id)
{
    validation_service->validate_id(id);
    auto client = client_repository_->get_by_id(id);
    client_repository_->remove(client->get_id());
}
void ClientService::update(int id, Client* new_client) const
{
    validation_service->validate_id(id);
    validation_service->validate_name(new_client->get_name());
    validation_service->validate_id(new_client->get_social_status_id());
    if(!social_status_service_->exists(new_client->get_social_status_id())){
        throw ValidationException("Social status does not exist");
    }

    auto client = client_repository_->get_by_id(id);
    client->set_name(new_client->get_name());
    client->set_social_status_id(new_client->get_social_status_id());
    client->set_is_admin(new_client->get_is_admin());
    client_repository_->update(client.get());
}
std::unique_ptr<Client> ClientService::get_by_id(int id) const
{
    validation_service->validate_id(id);
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
    validation_service->validate_id(id);
    return client_repository_->exists(id);
}
std::vector<std::unique_ptr<Client>> ClientService::get_all_by_bank_id(const int bank_id)
{
    validation_service->validate_id(bank_id);
    return client_repository_->get_all_by_bank_id(bank_id);
}
bool ClientService::has_accounts(const int id) const
{
    validation_service->validate_id(id);
    return client_repository_->has_accounts(id);
}