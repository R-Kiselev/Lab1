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

    BankRepository* bank_repository;
    BankService* bank_service;
    SocialStatusRepository* social_status_repository;
    SocialStatusService* social_status_service;
    ClientRepository* client_repository;
    ClientService* client_service;
    AccountRepository* account_repository;
    AccountService* account_service;
    CardRepository* card_repository;
    CardService* card_service;

    TransactionService* transaction_service;
    sqlite3* db_;
};


#endif //COURSE_WORK_TRANSACTION_DIALOG_H
