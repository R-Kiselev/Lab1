#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include "account_dialog.h"
#include "../../sqlite/sqlite3.h"
#include "../Account/AccountService.h"
#include "account_widget.h"
#include "cards_window.h"
#include "clients_window.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class accounts_window; }
QT_END_NAMESPACE

class accounts_window : public QWidget {
    Q_OBJECT

public:
    explicit accounts_window(sqlite3* db);
    ~accounts_window() override;

    void setup_services(sqlite3* db);

    void load_accounts(int client_id);

    void add();
    void update(int account_id);
    void delete_account(int account_id);

    void set_client_id(int client_id);
    void set_bank_id(int bank_id);
signals:
    void back_button();

private slots:
    void on_back_button_clicked();
    void open_card_window(int account_id);

private:
    Ui::accounts_window *ui;
    QVBoxLayout *layout;

    sqlite3* db_;
    cards_window* cards_window_;

    BankRepository* bank_repository;
    BankService* bank_service;
    SocialStatusRepository* social_status_repository;
    SocialStatusService* social_status_service;
    ClientRepository* client_repository;
    ClientService* client_service;
    AccountRepository* account_repository;
    AccountService* account_service;

    int client_id;
    int bank_id;
};

