//
// Created by Waimale on 11/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_payments_window.h" resolved

#include "../../include/ui/payments_window.h"
#include "ui_payments_window.h"


payments_window::payments_window(sqlite3* db) :
        QWidget(nullptr), ui(new Ui::payments_window), db_(db) {
    ui->setupUi(this);
    setWindowTitle("Payments");
    setMinimumSize(400, 300);

    connect(ui->back_button, &QPushButton::clicked, this, &payments_window::go_back);

    transaction_dialog_ = new transaction_dialog(db_);
    connect(ui->transfer_accounts_button, &QPushButton::clicked, [this]() { open_transaction_dialog(0); });
    connect(ui->transfer_account_card_button, &QPushButton::clicked, [this]() { open_transaction_dialog(1); });
    connect(ui->transfer_cards_button, &QPushButton::clicked, [this]() { open_transaction_dialog(2); });
}

payments_window::~payments_window() {
    delete ui;
}
void payments_window::go_back() {
    this->close();
    emit back_button();
}

void payments_window::open_transaction_dialog(int transfer_type) {
    transaction_dialog_->set_transfer_type(transfer_type);
    transaction_dialog_->exec();
}
