//
// Created by Waimale on 10/30/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_account_widget.h" resolved

#include "../../include/ui/account_widget.h"
#include "ui_account_widget.h"
#include <QMouseEvent>

account_widget::account_widget(QWidget *parent, Account* account) :
        QWidget(parent), ui(new Ui::account_widget), account(account) {
    account_id = account ? account->get_id() : -1;

    ui->setupUi(this);
    ui->labelId->setText("Id: " + QString::number(account->get_id()));
    ui->labelBalance->setText("Balance: " + QString::number(account->get_balance()));
    ui->labelClientId->setText("Client id: " + QString::number(account->get_client_id()));
    ui->labelBankId->setText("Bank id: " + QString::number(account->get_bank_id()));

    connect(ui->update_button, &QPushButton::clicked, this, &account_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &account_widget::onDeleteClicked);
}

account_widget::~account_widget() {
}

void account_widget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(account_id);
    }
}
void account_widget::onUpdateClicked() {
    emit updateRequested(account_id);
}
void account_widget::onDeleteClicked() {
    emit deleteRequested(account_id);
}