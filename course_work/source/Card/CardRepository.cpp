#include "../../include/Card/CardRepository.h"


void CardRepository::add(Card* card) const
{
    std::string sql = "INSERT INTO cards (number, expire_date, balance, account_id) VALUES (?, ?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for adding card");
    }
    sqlite3_bind_text(stmt, 1, card->get_number().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, card->get_expire_date().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, card->get_balance());
    sqlite3_bind_int(stmt, 4, card->get_account_id());
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to insert card into database");
    }
    // Delete?
    int last_id = int(sqlite3_last_insert_rowid(db_));
    card->set_id(last_id);
    sqlite3_finalize(stmt);
}
void CardRepository::remove(const int id) {
    std::string sql = "DELETE FROM cards WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for removing card");
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to remove card from database");
    }
    sqlite3_finalize(stmt);
}
std::unique_ptr<Card> CardRepository::get_by_id(const int id) const
{
    std::string sql = "SELECT id, number, expire_date, balance, account_id FROM cards WHERE id = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting card by ID");
    }
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException("Card with ID " + std::to_string(id) + " not found in database");
    }
    int card_id = sqlite3_column_int(stmt, 0);
    std::string number(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string expire_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    int balance = sqlite3_column_int(stmt, 3);
    int account_id = sqlite3_column_int(stmt, 4);

    auto card = std::make_unique<Card>(number, expire_date, balance, account_id);
    card->set_id(card_id);

    return card;
}
std::unique_ptr<Card> CardRepository::get_card_by_number(std::string& number_) const{
    std::string sql = "SELECT id, number, expire_date, balance, account_id FROM cards WHERE number = ?;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting card by number");
    }
    sqlite3_bind_text(stmt, 1, number_.c_str(), -1, SQLITE_STATIC);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException("Card with number " + number_ + " not found in database");
    }
    int card_id = sqlite3_column_int(stmt, 0);
    std::string number(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string expire_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    int balance = sqlite3_column_int(stmt, 3);
    int account_id = sqlite3_column_int(stmt, 4);

    auto card = std::make_unique<Card>(number, expire_date, balance, account_id);
    card->set_id(card_id);

    return card;
}
std::vector<std::unique_ptr<Card>> CardRepository::get_all() const {
    std::vector<std::unique_ptr<Card>> cards;
    std::string sql = "SELECT id, number, expire_date, balance, account_id FROM cards;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting card by ID");
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        auto card = std::make_unique<Card>();

        card->set_id(sqlite3_column_int(stmt, 0));
        card->set_number(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        card->set_expire_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        card->set_balance(sqlite3_column_int(stmt, 3));
        card->set_account_id(sqlite3_column_int(stmt, 4));

        cards.push_back(std::move(card));
    }
    sqlite3_finalize(stmt);
    if(cards.empty()){
        throw NotFoundException("No cards found in database");
    }

    return cards;
}
std::vector<std::unique_ptr<Card>> CardRepository::get_cards_by_account_id(const int account_id) const
{
    std::vector<std::unique_ptr<Card>> cards;
    std::string sql = "SELECT id, number, expire_date, balance, account_id "
                      "FROM cards "
                      "WHERE account_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting card by ID");
    }
    sqlite3_bind_int(stmt, 1, account_id);
    while(sqlite3_step(stmt) == SQLITE_ROW){
        auto card = std::make_unique<Card>();

        card->set_id(sqlite3_column_int(stmt, 0));
        card->set_number(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        card->set_expire_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        card->set_balance(sqlite3_column_int(stmt, 3));
        card->set_account_id(sqlite3_column_int(stmt, 4));

        cards.push_back(std::move(card));
    }
    sqlite3_finalize(stmt);
    if(cards.empty()){
        throw NotFoundException("No cards found in database");
    }

    return cards;
}

void CardRepository::update(Card *card) const {
    std::string sql = "UPDATE cards SET balance = ?, account_id = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        throw DatabaseException("Failed to prepare SQL statement for updating card");
    }
    sqlite3_bind_int(stmt, 1, card->get_balance());
    sqlite3_bind_int(stmt, 2, card->get_account_id());
    sqlite3_bind_int(stmt, 3, card->get_id());
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to update card in database");
    }
    sqlite3_finalize(stmt);
}
bool CardRepository::exists(const int id) const
{
    std::string sql = "SELECT EXISTS(SELECT 1 FROM cards WHERE id = ?);";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking card existence");
    }

    sqlite3_bind_int(stmt, 1, id);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return exists;
}
bool CardRepository::exists(std::string number) const
{
    std::string sql = "SELECT EXISTS(SELECT 1 FROM cards WHERE number = ?);";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for checking card existence");
    }

    sqlite3_bind_text(stmt, 1, number.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return exists;
}
