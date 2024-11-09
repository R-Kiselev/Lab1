#include "../../include/Account/AccountService.h"

AccountService::AccountService(AccountRepository *account_repository,
                               ClientService *client_service,
                               BankService *bank_service)
    : account_repository_(account_repository),
      client_service_(client_service),
      bank_service_(bank_service)
{
}

void AccountService::add(const int client_id, const int bank_id) const {
    if(!client_service_->exists(client_id)){
        throw ValidationException("Client does not exist");
    }
    if(!bank_service_->exists(bank_id)){
        throw ValidationException("Bank does not exist");
    }
    auto account = std::make_unique<Account>(client_id, bank_id);
    account_repository_->add(account.get());
}
void AccountService::remove(int id) {
    try {
        auto account = account_repository_->get_by_id(id);
        account_repository_->remove(account->get_id());
    } catch (const NotFoundException& e) {
        throw;
    } catch (const CustomException& e) {
        throw;
    }
}
void AccountService::update(int id, const int balance)
{
    if (balance < 0){
        throw ValidationException("Balance cannot be negative");
    }
    try {
        auto account = account_repository_->get_by_id(id);
        account->set_balance(balance);
        account_repository_->update(account.get());
    } catch (const NotFoundException& e) {
        throw;
    } catch (const CustomException& e) {
        throw;
    }
}
std::unique_ptr<Account> AccountService::get_by_id(int id) const
{
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
    return account_repository_->exists(id);
}

std::vector<std::unique_ptr<Account>> AccountService::get_all_by_client_id(int client_id) const
{
    return account_repository_->get_all_by_client_id(client_id);
}