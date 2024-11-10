//
// Created by Waimale on 10/31/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_bank_window.h" resolved

#include "../../include/ui/bank_window.h"
#include "ui_bank_window.h"
#include <QInputDialog>
#include <QMessageBox>

bank_window::bank_window(sqlite3* db) :
        QWidget(nullptr), ui(new Ui::bank_window), db(db) {
    ui->setupUi(this);
    setWindowTitle("Список банков");

    clients_window_ = std::make_unique<clients_window>(db);
    connect(clients_window_.get(), &clients_window::back_button, this, &bank_window::show);
    connect(ui->back_button, &QPushButton::clicked, this, &bank_window::go_back);

    connect(ui->add_button, &QPushButton::clicked, this, &bank_window::add);

    setup_services(db);
}

bank_window::~bank_window() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

void bank_window::go_back() {
    this->close();
    emit back_button();
}

void bank_window::setup_services(sqlite3* db) {
    bank_repository = std::make_unique<BankRepository>(db);
    bank_service = std::make_unique<BankService>(bank_repository.get());
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
        bank_service->add(bank_name.toStdString());
        load_banks();
    }
}
void bank_window::update(int bank_id) {
    bool ok;
    QString new_name = QInputDialog::getText(this, tr("Update Bank"),
                                             tr("Enter new bank name:"), QLineEdit::Normal, "", &ok);
    if (ok) {
        try{
            bank_service->update(bank_id, new_name.toStdString());
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
    auto *container = new QWidget(this);
    auto layout {new QVBoxLayout(container)};

    try{
        auto banks = bank_service->get_all();
        for (const auto& bank : banks) {
            auto *bank_widget_ = new bank_widget(bank.get(), this);
            bank_widget_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

            connect(bank_widget_, &bank_widget::clicked, this, &bank_window::open_clients_window);
            connect(bank_widget_, &bank_widget::updateRequested, this, &bank_window::update);
            connect(bank_widget_, &bank_widget::deleteRequested, this, &bank_window::delete_bank);

            layout->addWidget(bank_widget_);
        }
    }
    catch (const CustomException& e) {
        QMessageBox::information(this, "Information", "Banks was not found.");
    }

    container->setLayout(layout);
    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    layout->setSpacing(10);
}