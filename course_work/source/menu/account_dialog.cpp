//
// Created by Waimale on 11/5/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_account_dialog.h" resolved

#include "../../include/ui/account_dialog.h"
#include "ui_account_dialog.h"


account_dialog::account_dialog(AccountService* account_service, QWidget *parent) :
        QDialog(parent), ui(new Ui::account_dialog) {
    ui->setupUi(this);
    connect(ui->ok_button, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel_button, &QPushButton::clicked, this, &QDialog::reject);

}

account_dialog::~account_dialog() {
    delete ui;
}
std::unique_ptr<Account> account_dialog::get_account() const {
    auto account = std::make_unique<Account>();
    account->set_id(ui->id_edit->text().toInt());
    account->set_balance(ui->balance_edit->text().toDouble());
    account->set_client_id(ui->client_id_edit->text().toInt());
    account->set_bank_id(ui->bank_id_edit->text().toInt());
    return account;
}