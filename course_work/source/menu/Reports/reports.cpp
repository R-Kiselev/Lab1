#include "../../../include/ui/reports.h"
#include "ui_reports.h"
#include <QMessageBox>

reports::reports(sqlite3* db, QWidget *parent) :
        QWidget(parent), ui(new Ui::reports), db_(db) {
    ui->setupUi(this);

    populateBanks();
    connect(ui->back_button, &QPushButton::clicked, this, [this](){
        this->close();
        emit back_button_clicked();
    });
    connect(ui->pushButton_open_clients_report, &QPushButton::clicked, this, &reports::openClientsReport);
    connect(ui->pushButton_open_accounts_and_cards_report, &QPushButton::clicked, this, &reports::openAccountsAndCardsReport);
}

reports::~reports() {
    delete ui;
}

void reports::populateBanks() {
    const char* query = "SELECT id, name FROM banks";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            QString name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            ui->bankComboBox->addItem(name, id);
        }
        sqlite3_finalize(stmt);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить банки из базы данных.");
    }
}

void reports::openClientsReport() {
    int selectedBankId = ui->bankComboBox->currentData().toInt();
    clients_report_window_ = std::make_unique<clients_report>(db_, selectedBankId);
    connect(clients_report_window_.get(), &clients_report::back_button_clicked, this, &reports::show);
    clients_report_window_->show();
    this->close();
}
void reports::openAccountsAndCardsReport() {
    int selectedBankId = ui->bankComboBox->currentData().toInt();
    accounts_and_cards_report_window_ = std::make_unique<accounts_and_cards_report>(db_, selectedBankId);
    connect(accounts_and_cards_report_window_.get(), &accounts_and_cards_report::back_button_clicked, this, &reports::show);
    accounts_and_cards_report_window_->show();
    this->close();
}
