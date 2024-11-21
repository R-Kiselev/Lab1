//
// Created by Waimale on 10/30/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_account_widget.h" resolved

#include "../../../include/ui/account_widget.h"
#include "ui_account_widget.h"
#include <QMouseEvent>

account_widget::account_widget(QWidget *parent, Account* account, sqlite3* db) :
        QWidget(parent), ui(new Ui::account_widget), account(account), db_(db) {
    set_account_id(account->get_id());
    ui->setupUi(this);
    ui->labelId->setText("Id: " + QString::number(account->get_id()));
    ui->labelBalance->setText("Balance: " + QString::number(account->get_balance()));

    std::string client_name = get_client_name(account->get_client_id());
    std::string bank_name = get_bank_name(account->get_bank_id());

    ui->labelClientId->setText("Client name: " + QString::fromStdString(client_name));
    ui->labelBankId->setText("Bank name: " + QString::fromStdString(bank_name));

    connect(ui->update_button, &QPushButton::clicked, this, &account_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &account_widget::onDeleteClicked);
}
std::string account_widget::get_client_name(int client_id) {
    std::string sql = std::format("SELECT name FROM clients WHERE id = {};", client_id);
    sqlite3_stmt* stmt;
    std::string name;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Client with ID {} not found", client_id));
    }

    sqlite3_finalize(stmt);
    return name;
}

std::string account_widget::get_bank_name(int bank_id) {
    std::string sql = std::format("SELECT name FROM banks WHERE id = {};", bank_id);
    sqlite3_stmt* stmt;
    std::string name;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Bank with ID {} not found", bank_id));
    }

    sqlite3_finalize(stmt);
    return name;
}

account_widget::~account_widget() = default;
void account_widget::set_account_id(int account_id_){
    account_id = account_id_;
}
void account_widget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(account_id);
    }
}
void account_widget::onUpdateClicked() {
    emit updateRequested(account_id);
}
void account_widget::onDeleteClicked() {
    emit deleteRequested(account_id);
}