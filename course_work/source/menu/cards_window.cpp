//
// Created by Waimale on 10/30/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_cards_window.h" resolved

#include "../../include/ui/cards_window.h"
#include "ui_cards_window.h"

cards_window::cards_window(sqlite3* db) :
        QWidget(nullptr), ui(new Ui::cards_window), db_(db) {
    ui->setupUi(this);

    setWindowTitle("Список карт, привязанных к аккаунту");
    setMinimumSize(400, 300);

    connect(ui->back_button, &QPushButton::clicked, this, &cards_window::go_back);
    connect(ui->add_button, &QPushButton::clicked, this, &cards_window::add);

    setup_services(db_);
}

cards_window::~cards_window() {
    delete ui;
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
    delete card_service;
    delete card_repository;
    delete account_service;
    delete account_repository;
    delete client_service;
    delete client_repository;
    delete social_status_service;
    delete social_status_repository;
    delete bank_service;
    delete bank_repository;
}

void cards_window::go_back() {
    this->close();
    emit back_button();
}

void cards_window::setup_services(sqlite3* db){
    bank_repository = new BankRepository(db);
    bank_service = new BankService(bank_repository);

    social_status_repository = new SocialStatusRepository(db);
    social_status_service = new SocialStatusService(social_status_repository);

    client_repository = new ClientRepository(db);
    client_service = new ClientService(client_repository, social_status_service);

    account_repository = new AccountRepository(db);
    account_service = new AccountService(account_repository, client_service, bank_service);

    card_repository = new CardRepository(db);
    card_service = new CardService(card_repository, account_service);
}
void cards_window::add() {
    try {
        card_service->add(account_id_);
        load_cards(account_id_);
    } catch (const CustomException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
void cards_window::update(int card_id) {
    bool ok;
    QString new_balance = QInputDialog::getText(this, tr("Update Card"),
                                                tr("Enter new card balance:"), QLineEdit::Normal, "", &ok);
    if (ok) {
        try {
            card_service->update(card_id, new_balance.toInt());
            load_cards(account_id_);
        } catch (const CustomException &e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void cards_window::delete_card(int card_id) {
    int reply = QMessageBox::question(this, tr("Delete Card"),
                                      tr("Are you sure you want to delete this card?"),
                                      QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            card_service->remove(card_id);
            load_cards(account_id_);
        } catch (const CustomException &e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}
void cards_window::load_cards(int account_id) {
    auto *container = new QWidget(this);
    auto *layout = new QVBoxLayout(container);

    try{
        auto cards = card_service->get_cards_by_account_id(account_id);
        for (const auto& card : cards) {
            auto *card_widget_ = new card_widget(this, card.get());
            connect(card_widget_, &card_widget::updateRequested, this, &cards_window::update);
            connect(card_widget_, &card_widget::deleteRequested, this, &cards_window::delete_card);
            card_widget_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            layout->addWidget(card_widget_);
        }
    }
    catch (const CustomException& e) {
        QMessageBox::information(this, "Information", "Cards was not found.");
    }

    container->setLayout(layout);
    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);
}
void cards_window::set_account_id(int account_id){
    account_id_ = account_id;
}