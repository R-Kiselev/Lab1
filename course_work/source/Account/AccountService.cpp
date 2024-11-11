#include "../../include/Account/AccountService.h"

AccountService::AccountService(AccountRepository *account_repository,
                               ClientService *client_service,
                               BankService *bank_service)
    : account_repository_(account_repository),
      client_service_(client_service),
      bank_service_(bank_service)
{
    validation_service = std::make_unique<ValidationService>();
}

void AccountService::add(Account* account) const {
    validation_service->validate_id(account->get_client_id());
    validation_service->validate_id(account->get_bank_id());
    if(!client_service_->exists(account->get_client_id())){
        throw ValidationException("Client does not exist");
    }
    if(!bank_service_->exists(account->get_bank_id())){
        throw ValidationException("Bank does not exist");
    }
    account_repository_->add(account);
}
void AccountService::remove(int id) {
    validation_service->validate_id(id);
    auto account = account_repository_->get_by_id(id);
    account_repository_->remove(account->get_id());
}
void AccountService::update(int id, Account* new_account) const
{
    validation_service->validate_balance(new_account->get_balance());

    auto account = account_repository_->get_by_id(id);
    account->set_balance(new_account->get_balance());

    account_repository_->update(account.get());
}
std::unique_ptr<Account> AccountService::get_by_id(int id) const
{
    validation_service->validate_id(id);
    return account_repository_->get_by_id(id);
}
std::vector<std::unique_ptr<Account>> AccountService::get_all() const
{
    return account_repository_->get_all();
}
void AccountService::display_all() const {
    auto accounts = account_repository_->get_all();
    if (accounts.empty()) {
        std::cout << "No accounts available." << std::endl;
    } else {
        for (const auto& account : accounts) {
            std::cout << "Account ID: " << account->get_id()
                      << ", Balance: " << account->get_balance()
                      << ", Client ID: " << account->get_client_id()
                      << ", Bank ID: " << account->get_bank_id()
                      << std::endl;
        }
        std::cout << std::endl;
    }
}
bool AccountService::exists(const int id) const {
    validation_service->validate_id(id);
    return account_repository_->exists(id);
}

std::vector<std::unique_ptr<Account>> AccountService::get_all_by_client_id(int client_id) const
{
    validation_service->validate_id(client_id);
    return account_repository_->get_all_by_client_id(client_id);
}