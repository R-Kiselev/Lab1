#include "../../include/Bank/BankService.h"

BankService::BankService(BankRepository *bank_repository)
    : bank_repository_(bank_repository){}
// improve validation
void BankService::add(const std::string &name) const {
    if (name.empty()) {
        throw ValidationException("Bank name cannot be empty");
    }

    auto bank = std::make_unique<Bank>(name);
    bank_repository_->add(bank.get());
}
void BankService::remove(int id) {
    try {
        auto bank = bank_repository_->get_by_id(id);
        bank_repository_->remove(bank->get_id());
    } catch (const NotFoundException& e) {
        throw;
    } catch (const CustomException& e) {
        throw;
    }
}
void BankService::update(const int id, const std::string& new_name) {
    if (new_name.empty()) {
        throw ValidationException("New bank name cannot be empty");
    }

    try {
        auto bank = bank_repository_->get_by_id(id);
        bank->set_name(new_name);
        bank_repository_->update(bank.get());
    } catch (const NotFoundException& e) {
        throw;
    } catch (const CustomException& e) {
        throw;
    }
}

std::unique_ptr<Bank> BankService::get_by_id(const int id) const
{
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
    return bank_repository_->exists(id);
}
