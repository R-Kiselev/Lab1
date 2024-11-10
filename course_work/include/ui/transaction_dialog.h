//
// Created by Waimale on 11/8/2024.
//

#ifndef COURSE_WORK_TRANSACTION_DIALOG_H
#define COURSE_WORK_TRANSACTION_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include "../Transaction/TransactionService.h"
#include "../../sqlite/sqlite3.h"
#include "../Card/CardService.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class transaction_dialog; }
QT_END_NAMESPACE

class transaction_dialog : public QDialog {
Q_OBJECT

public:
    explicit transaction_dialog(sqlite3* db);

    ~transaction_dialog() override;

    void set_transfer_type(int transferType);
    void setup_services();
public slots:
    void perform_transaction();
private:
    void handle_transfer_between_accounts(int source_id, int target_id, int amount);
    void handle_transfer_between_cards(std::string source_number, std::string target_number, int amount);
    void handle_account_to_card_transfer(int source_id, std::string target_card_number, int amount);
    void handle_card_to_account_transfer(std::string source_card_number, int target_id, int amount);

    void begin_transaction();
    void commit_transaction();
    void rollback_transaction();

    Ui::transaction_dialog *ui;
    int transfer_type;

    std::unique_ptr<BankRepository> bank_repository;
    std::unique_ptr<BankService> bank_service;
    std::unique_ptr<SocialStatusRepository> social_status_repository;
    std::unique_ptr<SocialStatusService> social_status_service;
    std::unique_ptr<ClientRepository> client_repository;
    std::unique_ptr<ClientService> client_service;
    std::unique_ptr<AccountRepository> account_repository;
    std::unique_ptr<AccountService> account_service;
    std::unique_ptr<CardRepository> card_repository;
    std::unique_ptr<CardService> card_service;

    std::unique_ptr<TransactionService> transaction_service;
    sqlite3* db_;
};


#endif //COURSE_WORK_TRANSACTION_DIALOG_H
