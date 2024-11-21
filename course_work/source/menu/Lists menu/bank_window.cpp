//
// Created by Waimale on 10/31/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_bank_window.h" resolved

#include "../../../include/ui/bank_window.h"
#include "ui_bank_window.h"
#include <QInputDialog>
#include <QMessageBox>

bank_window::bank_window(sqlite3* db, int user_id) :
        QWidget(nullptr), ui(new Ui::bank_window), db_(db), user_id_(user_id) {
    ui->setupUi(this);
    setWindowTitle("Список банков");

    is_admin_ = get_is_admin();

    if (is_admin_){
        clients_window_ = std::make_unique<clients_window>(db);
        connect(clients_window_.get(), &clients_window::back_button, this, &bank_window::show);
    }
    else{
        accounts_window_ = std::make_unique<accounts_window>(db);
        connect(accounts_window_.get(), &accounts_window::back_button, this, &bank_window::show);
        ui->add_button->hide();
    }

    connect(ui->back_button, &QPushButton::clicked, this, &bank_window::go_back);
    connect(ui->add_button, &QPushButton::clicked, this, &bank_window::add);

    setup_services(db_);
}

bank_window::~bank_window() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool bank_window::get_is_admin() const {
    std::string sql = std::format("SELECT is_admin FROM clients WHERE id = {};", std::to_string(user_id_));
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Client with ID {} not found", std::to_string(user_id_)));
    }
    int is_admin = (sqlite3_column_int(stmt, 0) == 1);
    sqlite3_finalize(stmt);
    return is_admin;
}

void bank_window::go_back() {
    this->close();
    emit back_button();
}

void bank_window::setup_services(sqlite3* db_) {
    bank_repository = std::make_unique<BankRepository>(db_);
    bank_service = std::make_unique<BankService>(bank_repository.get());
}
void bank_window::open_accounts_window(int bank_id) {
    this->close();
    accounts_window_->set_bank_id(bank_id);
    accounts_window_->set_client(user_id_);
    accounts_window_->load_accounts(user_id_);
    accounts_window_->show();
}
void bank_window::open_clients_window(int bank_id) {
    this->close();
    clients_window_->setBankId(bank_id);
    clients_window_->load_clients(bank_id);
    clients_window_->show();
}
void bank_window::add(int bank_id) {
    bool ok;
    QString bank_name = QInputDialog::getText(this, tr("Add Bank"),
                                              tr("Enter bank name:"), QLineEdit::Normal, "", &ok);
    if (ok && !bank_name.isEmpty()) {
        try{
            auto bank = std::make_unique<Bank>(bank_name.toStdString());
            bank_service->add(bank.get());
            load_banks();
        }
        catch (CustomException& e){
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void bank_window::update(int bank_id) {
    bool ok;
    QString new_name = QInputDialog::getText(this, tr("Update Bank"),
                                             tr("Enter new bank name:"), QLineEdit::Normal, "", &ok);
    if (ok) {
        try{
            auto bank = std::make_unique<Bank>(new_name.toStdString());
            bank_service->update(bank_id, bank.get());
            load_banks();
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void bank_window::delete_bank(int bank_id) {
    int reply = QMessageBox::question(this, tr("Delete Bank"),
                                      tr("Are you sure you want to delete this bank?"),
                                      QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            bank_service->remove(bank_id);
            load_banks();
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void bank_window::load_banks() {
    auto container = std::make_unique<QWidget>(this);
    auto layout = std::make_unique<QVBoxLayout>(container.get());

    try{
        auto banks = bank_service->get_all();
        for (const auto& bank : banks) {
            auto bank_widget_ = std::make_unique<bank_widget>(bank.get(), this, is_admin_);
            bank_widget_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

           if(is_admin_){
               connect(bank_widget_.get(), &bank_widget::clicked, this, &bank_window::open_clients_window);
               connect(bank_widget_.get(), &bank_widget::updateRequested, this, &bank_window::update);
               connect(bank_widget_.get(), &bank_widget::deleteRequested, this, &bank_window::delete_bank);
           }
          else {
               connect(bank_widget_.get(), &bank_widget::clicked, this, &bank_window::open_accounts_window);
           }
            layout->addWidget(bank_widget_.release());
        }
    }
    catch (const CustomException& e) {
        QMessageBox::information(this, "Information", "Banks was not found.");
    }

    container->setLayout(layout.release());
    ui->scrollArea->setWidget(container.release());
    ui->scrollArea->setWidgetResizable(true);
}