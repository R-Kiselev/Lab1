//
// Created by Waimale on 10/25/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_accounts_window.h" resolved

#include <QLabel>
#include "../../include/ui/cards_window.h"
#include "../../include/ui/accounts_window.h"

#include "ui_accounts_window.h"


accounts_window::accounts_window(sqlite3* db)
        : QWidget(nullptr), ui(new Ui::accounts_window), db_(db)
{
    ui->setupUi(this);

    cards_window_ = std::make_unique<cards_window>(db_);
    connect(cards_window_.get(), &cards_window::back_button, this, &accounts_window::show);

    setWindowTitle("Список аккаунтов клиента");
    setMinimumSize(400, 300);

    connect(ui->back_button, &QPushButton::clicked, this, &accounts_window::on_back_button_clicked);
    connect(ui->add_button, &QPushButton::clicked, this, &accounts_window::add);
    setup_services(db_);
}

accounts_window::~accounts_window() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

void accounts_window::on_back_button_clicked() {
    try {
        if (!client_service->has_accounts(client_id)) {
            client_service->remove(client_id);
        }
    } catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }

    this->close();
    emit back_button();
}

void accounts_window::open_card_window(int account_id) {
    this->close();
    cards_window_->set_account_id(account_id);
    cards_window_->load_cards(account_id);
    cards_window_->show();
}

void accounts_window::setup_services(sqlite3* db){
    bank_repository = std::make_unique<BankRepository>(db);
    bank_service = std::make_unique<BankService>(bank_repository.get());

    social_status_repository = std::make_unique<SocialStatusRepository>(db);
    social_status_service = std::make_unique<SocialStatusService>(social_status_repository.get());

    client_repository = std::make_unique<ClientRepository>(db);
    client_service = std::make_unique<ClientService>(client_repository.get(), social_status_service.get());

    account_repository = std::make_unique<AccountRepository>(db);
    account_service = std::make_unique<AccountService>(account_repository.get(), client_service.get(), bank_service.get());
}
void accounts_window::add(){
    try{
        auto account = std::make_unique<Account>(client_id, bank_id);
        account_service->add(account.get());
        load_accounts(client_id);
    }
    catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }

}
void accounts_window::update_account(int account_id){
    bool ok;
    QString new_balance = QInputDialog::getText(this, tr("Update Account"),
                                                tr("Enter new account balance:"), QLineEdit::Normal, "", &ok);
    if (ok) {
        try{
            auto account = std::make_unique<Account>(client_id, bank_id);
            account->set_balance(new_balance.toInt());
            account_service->update(account_id, account.get());
            load_accounts(client_id);
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void accounts_window::delete_account(int account_id){
    int reply = QMessageBox::question(this, tr("Delete Account"),
                                      tr("Are you sure you want to delete this account?"),
                                      QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            account_service->remove(account_id);
            load_accounts(client_id);
        } catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void accounts_window::load_accounts(int client_id) {
    auto container = std::make_unique<QWidget>(this);
    auto layout = std::make_unique<QVBoxLayout>(container.get());

    try {
        auto accounts = account_service->get_all_by_client_id(client_id);
        for (const auto& account : accounts) {
            auto account_widget_ = std::make_unique<account_widget>(this, account.get());
            account_widget_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

            connect(account_widget_.get(), &account_widget::clicked, this, &accounts_window::open_card_window);
            connect(account_widget_.get(), &account_widget::updateRequested, this, &accounts_window::update_account);
            connect(account_widget_.get(), &account_widget::deleteRequested, this, &accounts_window::delete_account);

            layout->addWidget(account_widget_.release());
        }
    }
    catch (const CustomException& e) {
        QMessageBox::warning(this, "Warning", "This client has no more accounts. Create account or this client will be deleted.");
    }

    container->setLayout(layout.release());
    ui->scrollArea->setWidget(container.release());
    ui->scrollArea->setWidgetResizable(true);
}
void accounts_window::set_client_id(int client_id_) {
    this->client_id = client_id_;
}
void accounts_window::set_bank_id(int bank_id_) {
    this->bank_id = bank_id_;
}