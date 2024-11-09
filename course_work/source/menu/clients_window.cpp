//
// Created by Waimale on 10/28/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_clients_window.h" resolved

#include "../../include/ui/clients_window.h"
#include "ui_clients_window.h"
#include <QLabel>
#include <QPushButton>
#include "../../include/ui/accounts_window.h"

clients_window::clients_window(sqlite3* db)
        : QWidget(nullptr), ui(new Ui::clients_window), db_(db)
{
    ui->setupUi(this);

    accounts_window_ = new accounts_window(db_);
    connect(accounts_window_, &accounts_window::back_button, this, [this](){
        load_clients(bank_id);
        this->show();
    });

    setWindowTitle("Список клиентов банка");
    setMinimumSize(400, 300);

    connect(ui->back_button, &QPushButton::clicked, this, &clients_window::go_back);
    connect(ui->add_button, &QPushButton::clicked, this, &clients_window::add);
    setup_services(db_);
}

clients_window::~clients_window() {
    delete ui;
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
    delete account_service;
    delete account_repository;
    delete client_service;
    delete client_repository;
    delete social_status_service;
    delete social_status_repository;
    delete bank_service;
    delete bank_repository;
}

void clients_window::open_accounts_window(int client_id) {
    this->close();
    accounts_window_->set_bank_id(bank_id);
    accounts_window_->set_client_id(client_id);
    accounts_window_->load_accounts(client_id);
    accounts_window_->show();
}

void clients_window::go_back() {
    this->close();
    emit back_button();
}

void clients_window::setup_services(sqlite3* db){
    bank_repository = new BankRepository(db);
    bank_service = new BankService(bank_repository);

    social_status_repository = new SocialStatusRepository(db);
    social_status_service = new SocialStatusService(social_status_repository);

    client_repository = new ClientRepository(db);
    client_service = new ClientService(client_repository, social_status_service);

    account_repository = new AccountRepository(db);
    account_service = new AccountService(account_repository, client_service, bank_service);
}

void clients_window::add() {
    client_dialog dialog(social_status_service, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString client_name = dialog.get_name();
        int social_status_id = dialog.get_social_status_id();

        try{
            auto client = client_service->add(client_name.toStdString(), social_status_id);
            account_service->add(client->get_id(), bank_id);
            load_clients(bank_id);
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void clients_window::update(int client_id) {
    client_dialog dialog(social_status_service, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString new_name = dialog.get_name();
        int new_social_status_id = dialog.get_social_status_id();

        try {
            client_service->update(client_id, new_name.toStdString(), new_social_status_id);
            load_clients(bank_id);
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void clients_window::delete_client(int client_id) {
    int reply = QMessageBox::question(this, tr("Delete Client"),
                                      tr("Are you sure you want to delete this client?"),
                                      QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            client_service->remove(client_id);
            load_clients(bank_id);
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void clients_window::load_clients(int bank_id) {
    auto *container = new QWidget(this);
    auto *layout = new QVBoxLayout(container);

    try{
        auto clients = client_service->get_all_by_bank_id(bank_id);
        for (const auto& client : clients) {
            auto *client_widget_ = new client_widget(client.get(), this);
            client_widget_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            connect(client_widget_, &client_widget::clicked, this, &clients_window::open_accounts_window);
            connect(client_widget_, &client_widget::updateRequested, this, &clients_window::update);
            connect(client_widget_, &client_widget::deleteRequested, this, &clients_window::delete_client);
            layout->addWidget(client_widget_);
        }
    }
    catch (const CustomException& e) {
        QMessageBox::information(this, "Empty", e.what());
    }

    container->setLayout(layout);
    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);
}

void clients_window::setBankId(int bankId) {
    bank_id = bankId;
}