#include "../../include/Bank/BankService.h"

BankService::BankService(BankRepository *bank_repository)
    : bank_repository_(bank_repository)
{
    validation_service = std::make_unique<ValidationService>();
}
void BankService::add(Bank* bank) const {
    validation_service->validate_name(bank->get_name());
    bank_repository_->add(bank);
}
void BankService::remove(int id) {
    auto bank = bank_repository_->get_by_id(id);
    bank_repository_->remove(bank->get_id());
}
void BankService::update(const int id, Bank* new_bank) const {
    validation_service->validate_name(new_bank->get_name());

    auto bank = bank_repository_->get_by_id(id);
    bank->set_name(new_bank->get_name());

    bank_repository_->update(bank.get());
}

std::unique_ptr<Bank> BankService::get_by_id(const int id) const
{
    validation_service->validate_id(id);
    return bank_repository_->get_by_id(id);
}
std::vector<std::unique_ptr<Bank>> BankService::get_all() const
{
    return bank_repository_->get_all();
}
void BankService::display_all() const {
    auto banks = bank_repository_->get_all();
    if (banks.empty()) {
        std::cout << "No banks available." << std::endl;
    } else {
        for (const auto& bank : banks) {
            std::cout << "Bank ID: " << bank->get_id()
                      << ", Name: " << bank->get_name()
                      << std::endl;
        }
        std::cout << std::endl;
    }
}
bool BankService::exists(const int id) const {
    validation_service->validate_id(id);
    return bank_repository_->exists(id);
}
