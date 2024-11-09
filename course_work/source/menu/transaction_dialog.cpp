//
// Created by Waimale on 11/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_transaction_dialog.h" resolved

#include "../../include/ui/transaction_dialog.h"
#include "ui_transaction_dialog.h"


transaction_dialog::transaction_dialog(sqlite3* db) :
        QDialog(nullptr), ui(new Ui::transaction_dialog), db_(db) {
    ui->setupUi(this);

    connect(ui->ok_button, &QPushButton::clicked, this, &transaction_dialog::perform_transaction);
    connect(ui->cancel_button, &QPushButton::clicked, this, &QDialog::reject);

    setup_services();
}

void transaction_dialog::setup_services() {
    bank_repository = new BankRepository(db_);
    bank_service = new BankService(bank_repository);
    social_status_repository = new SocialStatusRepository(db_);
    social_status_service = new SocialStatusService(social_status_repository);
    client_repository = new ClientRepository(db_);
    client_service = new ClientService(client_repository, social_status_service);
    account_repository = new AccountRepository(db_);
    account_service = new AccountService(account_repository, client_service, bank_service);
    card_repository = new CardRepository(db_);
    card_service = new CardService(card_repository, account_service);
}
void transaction_dialog::set_transfer_type(int transferType) {
    transfer_type = transferType;

    if (transfer_type == 0) {
        ui->source_label->setText("Account ID (Source):");
        ui->target_label->setText("Account ID (Target):");
    } else if (transfer_type == 1) {
        ui->source_label->setText("Account ID or Card Number (Source):");
        ui->target_label->setText("Account ID or Card Number (Target):");
    } else if (transfer_type == 2) {
        ui->source_label->setText("Card Number (Source):");
        ui->target_label->setText("Card Number (Target):");
    }
    ui->source_label->setWordWrap(true);
    ui->target_label->setWordWrap(true);
}
void transaction_dialog::begin_transaction() {
    sqlite3_exec(db_, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void transaction_dialog::commit_transaction() {
    sqlite3_exec(db_, "COMMIT", nullptr, nullptr, nullptr);
}

void transaction_dialog::rollback_transaction() {
    sqlite3_exec(db_, "ROLLBACK", nullptr, nullptr, nullptr);
}
void transaction_dialog::handle_transfer_between_accounts(int source_id, int target_id, int amount) {
    begin_transaction();
    try {
        auto source_account = account_service->get_by_id(source_id);
        auto target_account = account_service->get_by_id(target_id);

        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));

        transaction_service = new TransactionService();
        transaction_service->perform_transaction(source_account, target_account, amount);

        account_service->update(source_account->get_id(), source_account->get_balance());
        account_service->update(target_account->get_id(), target_account->get_balance());

        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));
        delete transaction_service;
    } catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}
void transaction_dialog::handle_transfer_between_cards(std::string source_number,
                                                       std::string target_number, int amount) {
    begin_transaction();
    try {
        auto source_card = card_service->get_card_by_number(source_number);
        auto target_card = card_service->get_card_by_number(target_number);
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
        transaction_service = new TransactionService();
        transaction_service->perform_transaction(source_card, target_card, amount);

        card_service->update(source_card->get_id(), source_card->get_balance());
        card_service->update(target_card->get_id(), target_card->get_balance());
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
        delete transaction_service;
    } catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}
void transaction_dialog::handle_account_to_card_transfer(int source_id, std::string target_card_number,
                                                         int amount) {
    begin_transaction();
    try {
        auto source_account = account_service->get_by_id(source_id);
        auto target_card = card_service->get_card_by_number(target_card_number);
        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
        transaction_service = new TransactionService();
        transaction_service->perform_transaction(source_account, target_card, amount);

        account_service->update(source_account->get_id(), source_account->get_balance());
        card_service->update(target_card->get_id(), target_card->get_balance());
        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
        delete transaction_service;
    }
    catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}
void transaction_dialog::handle_card_to_account_transfer(std::string source_card_number, int target_id, int amount) {
    begin_transaction();
    try{
        auto source_card = card_service->get_card_by_number(source_card_number);
        auto target_account = account_service->get_by_id(target_id);
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));
        transaction_service = new TransactionService();
        transaction_service->perform_transaction(source_card, target_account, amount);

        card_service->update(source_card->get_id(), source_card->get_balance());
        account_service->update(target_account->get_id(), target_account->get_balance());
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));
        delete transaction_service;
    }
    catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}
void transaction_dialog::perform_transaction() {
    sqlite3_exec(db_, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    QString source = ui->source_field->text();
    QString target = ui->target_field->text();
    QString amount = ui->amount_field->text();

    if (source.isEmpty() || target.isEmpty() || amount.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled out.");
        return;
    }

    if (transfer_type == 0) {
        handle_transfer_between_accounts(source.toInt(), target.toInt(), amount.toInt());
    } else if (transfer_type == 2) {
        handle_transfer_between_cards(source.toStdString(), target.toStdString(), amount.toInt());
    } else {
        bool is_source_integer;
        int source_id = source.toInt(&is_source_integer);
        if (is_source_integer){
            handle_account_to_card_transfer(source_id, target.toStdString(), amount.toInt());
        }
        else{
            handle_card_to_account_transfer(source.toStdString(), target.toInt(), amount.toInt());
        }
    }
    QDialog::accept();
}
transaction_dialog::~transaction_dialog() {
    delete ui;
    if (db_) {
        sqlite3_close(db_);
    }
    delete bank_service;
    delete bank_repository;
    delete social_status_service;
    delete social_status_repository;
    delete client_service;
    delete client_repository;
    delete account_service;
    delete account_repository;
    delete card_service;
    delete card_repository;
}