//
// Created by Waimale on 11/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_transaction_dialog.h" resolved

#include "../../../include/ui/transaction_dialog.h"
#include "ui_transaction_dialog.h"


transaction_dialog::transaction_dialog(sqlite3* db, int user_id) :
        QDialog(nullptr), ui(new Ui::transaction_dialog), db_(db), user_id_(user_id) {
    ui->setupUi(this);

    connect(ui->ok_button, &QPushButton::clicked, this, &transaction_dialog::perform_transaction);
    connect(ui->cancel_button, &QPushButton::clicked, this, &QDialog::reject);

    setup_services();
}

void transaction_dialog::setup_services() {
    bank_repository = std::make_unique<BankRepository>(db_);
    bank_service = std::make_unique<BankService>(bank_repository.get());

    social_status_repository = std::make_unique<SocialStatusRepository>(db_);
    social_status_service = std::make_unique<SocialStatusService>(social_status_repository.get());

    client_repository = std::make_unique<ClientRepository>(db_);
    client_service = std::make_unique<ClientService>(client_repository.get(), social_status_service.get());

    account_repository = std::make_unique<AccountRepository>(db_);
    account_service = std::make_unique<AccountService>(account_repository.get(), client_service.get(), bank_service.get());

    card_repository = std::make_unique<CardRepository>(db_);
    card_service = std::make_unique<CardService>(card_repository.get(), account_service.get());
}
void transaction_dialog::set_transfer_type(int transferType) {
    transfer_type = transferType;
    ui->source_selector->clear();
    if (transfer_type == 0) {
        ui->source_label->setText("Account IBAN (Source):");
        ui->target_label->setText("Account IBAN (Target):");
        auto accounts = account_service->get_all_by_client_id(user_id_);
        for (const auto& account : accounts) {
            ui->source_selector->addItem(account->get_IBAN().c_str());
        }
    } else if (transfer_type == 1) {
        ui->source_label->setText("Account IBAN or Card Number (Source):");
        ui->target_label->setText("Account IBAN or Card Number (Target):");
        std::vector<std::unique_ptr<Account>> accounts;
        std::vector<std::unique_ptr<Card>> cards;
        bool has_accounts = true;
        bool has_cards = true;
        try {
            accounts = account_service->get_all_by_client_id(user_id_);
        } catch (const CustomException& e) {
            has_accounts = false;
        }
        try {
            cards = card_service->get_all_by_client_id(user_id_);
        } catch (const CustomException& e) {
            has_cards = false;
        }
        if (!has_accounts && !has_cards) {
            throw CustomException("No accounts or cards found for this client.");
        }
        for (const auto& account : accounts) {
            ui->source_selector->addItem(account->get_IBAN().c_str());
        }
        for (const auto& card : cards) {
            ui->source_selector->addItem(card->get_number().c_str());
        }
        ui->source_label->setWordWrap(true);
        ui->target_label->setWordWrap(true);
    } else if (transfer_type == 2) {
        ui->source_label->setText("Card Number (Source):");
        ui->target_label->setText("Card Number (Target):");
        auto cards = card_service->get_all_by_client_id(user_id_);
        for (const auto& card : cards) {
            ui->source_selector->addItem(card->get_number().c_str());
        }
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
void transaction_dialog::handle_transfer_between_accounts(std::string source_IBAN, std::string target_IBAN, int amount) {
    begin_transaction();
    try {
        auto source_account = account_service->get_by_IBAN(source_IBAN);
        auto target_account = account_service->get_by_IBAN(target_IBAN);

        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));

        transaction_service = std::make_unique<TransactionService>();
        transaction_service->perform_transaction(source_account, target_account, amount);

        account_service->update(source_IBAN, source_account->get_balance());
        account_service->update(target_IBAN, target_account->get_balance());

        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));
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
        auto source_card = card_service->get_by_number(source_number);
        auto target_card = card_service->get_by_number(target_number);
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
        transaction_service = std::make_unique<TransactionService>();
        transaction_service->perform_transaction(source_card, target_card, amount);

        card_service->update(source_card->get_id(), source_card.get());
        card_service->update(target_card->get_id(), target_card.get());
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
    } catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}
void transaction_dialog::handle_account_to_card_transfer(std::string source_IBAN, std::string target_card_number,
                                                         int amount) {
    begin_transaction();
    try {
        auto source_account = account_service->get_by_IBAN(source_IBAN);
        auto target_card = card_service->get_by_number(target_card_number);
        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
        transaction_service = std::make_unique<TransactionService>();
        transaction_service->perform_transaction(source_account, target_card, amount);

        account_service->update(source_account->get_id(), source_account.get());
        card_service->update(target_card->get_id(), target_card.get());
        QMessageBox::information(this, "Balances", "Source account balance: " + QString::number(source_account->get_balance()) +
                                                   "\nTarget card balance: " + QString::number(target_card->get_balance()));
    }
    catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}
void transaction_dialog::handle_card_to_account_transfer(std::string source_card_number, std::string target_IBAN, int amount) {
    begin_transaction();
    try{
        auto source_card = card_service->get_by_number(source_card_number);
        auto target_account = account_service->get_by_IBAN(target_IBAN);
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));
        transaction_service = std::make_unique<TransactionService>();
        transaction_service->perform_transaction(source_card, target_account, amount);

        card_service->update(source_card->get_id(), source_card.get());
        account_service->update(target_account->get_id(), target_account.get());
        QMessageBox::information(this, "Balances", "Source card balance: " + QString::number(source_card->get_balance()) +
                                                   "\nTarget account balance: " + QString::number(target_account->get_balance()));
    }
    catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
        rollback_transaction();
    }
    commit_transaction();
}

void transaction_dialog::perform_transaction() {
    sqlite3_exec(db_, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    QString source = ui->source_selector->currentText();
    QString target = ui->target_field->text();
    QString amount = ui->amount_field->text();

    if (source.isEmpty() || target.isEmpty() || amount.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled out.");
        return;
    }
    if (transfer_type == 0) {
        handle_transfer_between_accounts(source.toStdString(), target.toStdString(), amount.toInt());
    } else if (transfer_type == 2) {
        handle_transfer_between_cards(source.toStdString(), target.toStdString(), amount.toInt());
    } else {
        if(account_service->exists(source.toStdString())){
            handle_account_to_card_transfer(source.toStdString(), target.toStdString(), amount.toInt());
        } else if(card_service->exists(source.toStdString())){
            handle_card_to_account_transfer(source.toStdString(), target.toStdString(), amount.toInt());
        } else {
            QMessageBox::critical(this, "Error", "Source account or card does not exist.");
            return;
        }
    }
    QDialog::accept();
}
transaction_dialog::~transaction_dialog() {
    if (db_) {
        sqlite3_close(db_);
    }
}