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

    accounts_window_ = std::make_unique<accounts_window>(db_);
    connect(accounts_window_.get(), &accounts_window::back_button, this, [this](){
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
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
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
    bank_repository = std::make_unique<BankRepository>(db);
    bank_service = std::make_unique<BankService>(bank_repository.get());

    social_status_repository = std::make_unique<SocialStatusRepository>(db);
    social_status_service = std::make_unique<SocialStatusService>(social_status_repository.get());

    client_repository = std::make_unique<ClientRepository>(db);
    client_service = std::make_unique<ClientService>(client_repository.get(), social_status_service.get());

    account_repository = std::make_unique<AccountRepository>(db);
    account_service = std::make_unique<AccountService>(account_repository.get(), client_service.get(), bank_service.get());
}

void clients_window::add() {
    client_dialog dialog(social_status_service.get(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString client_name = dialog.get_name();
        int social_status_id = dialog.get_social_status_id();

        try{
            auto client = std::make_unique<Client>(client_name.toStdString(), social_status_id);
            client_service->add(client.get());
            auto account = std::make_unique<Account>(client->get_id(), bank_id);
            account_service->add(account.get());
            load_clients(bank_id);
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void clients_window::update_client(int client_id) {
    client_dialog dialog(social_status_service.get(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString new_name = dialog.get_name();
        int new_social_status_id = dialog.get_social_status_id();

        try {
            auto client = std::make_unique<Client>(new_name.toStdString(), new_social_status_id);
            client_service->update(client_id, client.get());
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
void clients_window::load_clients(int bank_id_) {
    auto container = std::make_unique<QWidget>(this);
    auto layout = std::make_unique<QVBoxLayout>(container.get());

    try{
        auto clients = client_service->get_all_by_bank_id(bank_id_);
        for (const auto& client : clients) {
            auto client_widget_ = std::make_unique<client_widget>(client.get());
            client_widget_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            connect(client_widget_.get(), &client_widget::clicked, this, &clients_window::open_accounts_window);
            connect(client_widget_.get(), &client_widget::updateRequested, this, &clients_window::update_client);
            connect(client_widget_.get(), &client_widget::deleteRequested, this, &clients_window::delete_client);
            layout->addWidget(client_widget_.release());
        }
    }
    catch (const CustomException& e) {
        QMessageBox::information(this, "Empty", e.what());
    }

    container->setLayout(layout.release());
    ui->scrollArea->setWidget(container.release());
    ui->scrollArea->setWidgetResizable(true);
}

void clients_window::setBankId(int bankId) {
    bank_id = bankId;
}