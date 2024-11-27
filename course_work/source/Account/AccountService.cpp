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

std::string generate_random_digits(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 9);

    std::string result;
    for (int i = 0; i < length; ++i) {
        result += '0' + dist(gen);
    }
    return result;
}

std::string generate_bank_code() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> digit_dist(0, 9);
    std::uniform_int_distribution<> letter_dist(0, 25);

    std::string bank_code;
    for (int i = 0; i < 2; ++i) {
        bank_code += 'A' + letter_dist(gen);
    }
    for (int i = 0; i < 2; ++i) {
        bank_code += '0' + digit_dist(gen);
    }

    return bank_code;
}

int calculate_check_digits(const std::string& country_code, const std::string& data) {
    std::string transformed = data;
    transformed += (country_code[0] - 'A' + 10);
    transformed += (country_code[1] - 'A' + 10);
    transformed += "00";
    std::string numeric_representation;
    for (char c : transformed) {
        if (std::isdigit(c)) {
            numeric_representation += c;
        } else {
            numeric_representation += std::to_string(c - 'A' + 10);
        }
    }
    long long mod = 0;
    for (char digit : numeric_representation) {
        mod = (mod * 10 + (digit - '0')) % 97;
    }
    return 98 - mod;
}

std::string generate_iban_by() {
    const std::string country_code = "BY";
    std::string bank_code = generate_bank_code();
    std::string account_number = generate_random_digits(20);
    std::string iban_data = bank_code + account_number;
    int check_digits = calculate_check_digits(country_code, iban_data);
    std::string iban = country_code + (check_digits < 10 ? "0" : "") + std::to_string(check_digits) + bank_code + account_number;
    return iban;
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

    std::string iban = generate_iban_by();
    while(account_repository_->exists(iban)){
        iban = generate_iban_by();
    }
    account->set_IBAN(iban);

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
bool AccountService::exists(const std::string& IBAN) const {
    return account_repository_->exists(IBAN);
}
void AccountService::update(std::string IBAN, int balance) const {
    auto account = account_repository_->get_by_IBAN(IBAN);
    account->set_balance(balance);
    account_repository_->update(account.get());
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
std::vector<std::unique_ptr<Account>> AccountService::get_all_by_bank_id(int bank_id) const
{
    validation_service->validate_id(bank_id);
    return account_repository_->get_all_by_bank_id(bank_id);
}
std::unique_ptr<Account> AccountService::get_by_IBAN(const std::string& IBAN) const
{
    return account_repository_->get_by_IBAN(IBAN);
}