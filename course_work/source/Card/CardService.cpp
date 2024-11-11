#include "../../include/Card/CardService.h"


CardService::CardService(CardRepository *card_repository, AccountService *account_service) :
        card_repository_(card_repository), account_service_(account_service)
{
    validation_service = std::make_unique<ValidationService>();
}
std::string generate_card_number() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(0, 9);

    std::string card_number;

    for (int i = 0; i < 16; ++i) {
        card_number += std::to_string(dist(gen));
        if ((i + 1) % 4 == 0 && i != 15) {
            card_number += '-';
        }
    }

    return card_number;
}
std::string generate_card_expiry_date() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time{};
    localtime_s(&local_time, &now_time);
    int current_month = local_time.tm_mon + 1;
    int current_year = local_time.tm_year + 1900;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(3, 8);
    int additional_years = dist(gen);

    int expiry_year = current_year + additional_years;

    std::string expiry_date = std::format("{}{}/{}", (current_month < 10 ? "0" : ""), std::to_string(current_month), std::to_string(expiry_year % 100));

    return expiry_date;
}
void CardService::add(Card* card) const {
    validation_service->validate_id(card->get_account_id());
    if (!account_service_->exists(card->get_account_id())) {
        throw NotFoundException("Account does not exist");
    }

    std::string number = generate_card_number();
    while(card_repository_->exists(number)){
        number = generate_card_number();
    }
    std::string expire_date = generate_card_expiry_date();

    card->set_number(number);
    card->set_expire_date(expire_date);
    card->set_balance(0);
    card_repository_->add(card);
}
void CardService::remove(int id) {
    validation_service->validate_id(id);
    auto card = card_repository_->get_by_id(id);
    card_repository_->remove(card->get_id());
}
void CardService::update(int id, Card* new_card) const
{
    validation_service->validate_id(id);
    validation_service->validate_balance(new_card->get_balance());
    auto card = card_repository_->get_by_id(id);
    card->set_balance(new_card->get_balance());
    card_repository_->update(card.get());
}
std::unique_ptr<Card> CardService::get_by_id(int id) const {
    validation_service->validate_id(id);
    return card_repository_->get_by_id(id);
}
std::unique_ptr<Card> CardService::get_card_by_number(std::string &number) const {
    return card_repository_->get_card_by_number(number);
}
std::vector<std::unique_ptr<Card>> CardService::get_all() const {
    return card_repository_->get_all();
}
std::vector<std::unique_ptr<Card>> CardService::get_cards_by_account_id(const int account_id) const
{
    validation_service->validate_id(account_id);
    if (!account_service_->exists(account_id)) {
        throw NotFoundException("Account does not exist");
    }
    return card_repository_->get_cards_by_account_id(account_id);
}

void CardService::display_all_cards() const {
    auto cards = card_repository_->get_all();
    if (cards.empty()) {
        std::cout << "No cards found" << std::endl;
        return;
    }
    for (const auto& card : cards) {
        std::cout << "Card ID: " << card->get_id() << std::endl;
        std::cout << "Card number: " << card->get_number() << std::endl;
        std::cout << "Expire date: " << card->get_expire_date() << std::endl;
        std::cout << "Balance: " << card->get_balance() << std::endl;
        std::cout << "Account ID: " << card->get_account_id() << std::endl;
        std::cout << std::endl;
    }
}
bool CardService::exists(const int id) const {
    validation_service->validate_id(id);
    return card_repository_->exists(id);
}

bool compare_balances(const Card& card1, const Card& card2) {
    return card1.get_balance() < card2.get_balance();
}
