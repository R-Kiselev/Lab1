#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
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
    void update_account(int account_id);
    void delete_account(int account_id);

    void set_client_id(int client_id_);
    void set_bank_id(int bank_id_);
signals:
    void back_button();

private slots:
    void on_back_button_clicked();
    void open_card_window(int account_id);

private:
    std::unique_ptr<Ui::accounts_window> ui;
    std::unique_ptr<QVBoxLayout> layout;

    sqlite3* db_;
    std::unique_ptr<cards_window> cards_window_;

    std::unique_ptr<BankRepository> bank_repository;
    std::unique_ptr<BankService> bank_service;
    std::unique_ptr<SocialStatusRepository> social_status_repository;
    std::unique_ptr<SocialStatusService> social_status_service;
    std::unique_ptr<ClientRepository> client_repository;
    std::unique_ptr<ClientService> client_service;
    std::unique_ptr<AccountRepository> account_repository;
    std::unique_ptr<AccountService> account_service;

    int client_id;
    int bank_id;
};

