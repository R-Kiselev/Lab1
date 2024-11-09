//
// Created by Waimale on 10/30/2024.
//

#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include "../Card/CardService.h"
#include "card_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class cards_window;
class card_widget;}
QT_END_NAMESPACE

class cards_window : public QWidget {
    Q_OBJECT

public:
    explicit cards_window(sqlite3* db);
    ~cards_window() override;

    void setup_services(sqlite3* db);

    void load_cards(int account_id);
    void set_account_id(int account_id);
signals:
    void back_button();

protected slots:
    void go_back();
    void add();
    void update(int card_id);
    void delete_card(int card_id);
private:
    Ui::cards_window *ui;
    QVBoxLayout *layout;

    sqlite3* db_;

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

    int account_id_;
};
