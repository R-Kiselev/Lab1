// clients_window.h

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include "client_dialog.h"
#include "../Errors/CustomExceptions.h"
#include "../Client/ClientService.h"
#include "../Account/AccountService.h"
#include "../Bank/BankService.h"
#include "client_widget.h"
#include "../../sqlite/sqlite3.h"
class accounts_window;

QT_BEGIN_NAMESPACE
namespace Ui { class clients_window; }
QT_END_NAMESPACE

class clients_window : public QWidget {
    Q_OBJECT

public:
    explicit clients_window(sqlite3* db);
    ~clients_window() override;

    void setup_services(sqlite3* db);

    void load_clients(int bank_id);

    void setBankId(int bank_id);
    void add();
    void update(int client_id);
    void delete_client(int client_id);
signals:
    void back_button();
public slots:
    void go_back();
    void open_accounts_window(int client_id);


private:
    Ui::clients_window *ui;
    QVBoxLayout *layout;

    accounts_window* accounts_window_;
    sqlite3* db_;

    SocialStatusRepository* social_status_repository;
    SocialStatusService* social_status_service;
    ClientRepository* client_repository;
    ClientService* client_service;
    BankRepository* bank_repository;
    BankService* bank_service;
    AccountRepository* account_repository;
    AccountService* account_service;

    int bank_id;
};
