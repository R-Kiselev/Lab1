//
// Created by Waimale on 10/30/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_card_widget.h" resolved

#include "../../include/ui/card_widget.h"
#include "ui_card_widget.h"
#include "../../include/Bank/BankRepository.h"
#include <format>

card_widget::card_widget(QWidget *parent, Card* card, sqlite3* db) :
        QWidget(parent), ui(new Ui::card_widget), card(card), card_id (card ? card->get_id() : -1), db_(db) {
    ui->setupUi(this);
    ui->labelNumber->setText("Number: " + QString::fromStdString(card->get_number()));
    ui->labelExpireDate->setText("Expire date: " + QString::fromStdString(card->get_expire_date()));
    ui->labelBalance->setText("Balance: " + QString::number(card->get_balance()));
    std::string client_name = get_name_by_account_id(card->get_account_id());
    ui->labelAccountId->setText("Client name: " + QString::fromStdString(client_name));

    ui->labelNumber->setTextInteractionFlags(Qt::TextSelectableByMouse);
    connect(ui->update_button, &QPushButton::clicked, this, &card_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &card_widget::onDeleteClicked);
}

card_widget::~card_widget() = default;

void card_widget::onUpdateClicked() {
    emit updateRequested(card_id);
}
void card_widget::onDeleteClicked() {
    emit deleteRequested(card_id);
}
std::string card_widget::get_name_by_account_id(int account_id) {
    std::string sql = std::format("SELECT clients.name FROM clients INNER JOIN accounts ON accounts.client_id = clients.id WHERE accounts.id = {};", std::to_string(account_id));
    sqlite3_stmt* stmt;
    std::string name;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Client with ID {} not found", std::to_string(account_id)));
    }

    sqlite3_finalize(stmt);
    return name;
}