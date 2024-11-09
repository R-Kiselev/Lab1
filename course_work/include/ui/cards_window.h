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
#include <memory>

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
    void update_card(int card_id);
    void delete_card(int card_id);
private:
    Ui::cards_window *ui;
    QVBoxLayout *layout;

    sqlite3* db_;

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

    int account_id_;
};
