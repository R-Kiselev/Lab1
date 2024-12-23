//
// Created by Waimale on 10/30/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_cards_window.h" resolved

#include "../../../include/ui/cards_window.h"
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
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

void cards_window::go_back() {
    this->close();
    emit back_button();
}

void cards_window::setup_services(sqlite3* db){
    bank_repository = std::make_unique<BankRepository>(db);
    bank_service = std::make_unique<BankService>(bank_repository.get());

    social_status_repository = std::make_unique<SocialStatusRepository>(db);
    social_status_service = std::make_unique<SocialStatusService>(social_status_repository.get());

    client_repository = std::make_unique<ClientRepository>(db);
    client_service = std::make_unique<ClientService>(client_repository.get(), social_status_service.get());

    account_repository = std::make_unique<AccountRepository>(db);
    account_service = std::make_unique<AccountService>(account_repository.get(), client_service.get(), bank_service.get());

    card_repository = std::make_unique<CardRepository>(db);
    card_service = std::make_unique<CardService>(card_repository.get(), account_service.get());
}
void cards_window::add() {
    try {
        auto card = std::make_unique<Card>();
        card->set_account_id(account_id_);
        card_service->add(card.get());
        load_cards(account_id_);
    } catch (const CustomException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
void cards_window::update_card(int card_id) {
    bool ok;
    QString new_balance = QInputDialog::getText(this, tr("Update Card"),
                                                tr("Enter new card balance:"), QLineEdit::Normal, "", &ok);
    if (ok) {
        try {
            auto card = std::make_unique<Card>();
            card->set_balance(new_balance.toInt());
            card_service->update(card_id, card.get());
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
    auto container = std::make_unique<QWidget>(this);
    auto layout = std::make_unique<QVBoxLayout>(container.get());

    bool ignore_exception = false;
    try{
        auto cards = card_service->get_all_by_account_id(account_id);
        for (const auto& card : cards) {
            auto card_widget_ = std::make_unique<card_widget>(this, card.get(), db_);
            connect(card_widget_.get(), &card_widget::updateRequested, this, &cards_window::update_card);
            connect(card_widget_.get(), &card_widget::deleteRequested, this, &cards_window::delete_card);
            card_widget_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            layout->addWidget(card_widget_.release());
        }
    }
    catch (const CustomException& e) {
        ignore_exception = true;
    }

    container->setLayout(layout.release());
    ui->scrollArea->setWidget(container.release());
    ui->scrollArea->setWidgetResizable(true);
}
void cards_window::set_account_id(int account_id){
    account_id_ = account_id;
}