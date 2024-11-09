//
// Created by Waimale on 10/30/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_card_widget.h" resolved

#include "../../include/ui/card_widget.h"
#include "ui_card_widget.h"


card_widget::card_widget(QWidget *parent, Card* card) :
        QWidget(parent), ui(new Ui::card_widget), card(card), card_id (card ? card->get_id() : -1) {
    ui->setupUi(this);
    ui->labelId->setText("Id: " + QString::number(card->get_id()));
    ui->labelNumber->setText("Number: " + QString::fromStdString(card->get_number()));
    ui->labelExpireDate->setText("Expire date: " + QString::fromStdString(card->get_expire_date()));
    ui->labelBalance->setText("Balance: " + QString::number(card->get_balance()));
    ui->labelAccountId->setText("Bank id: " + QString::number(card->get_account_id()));

    ui->labelNumber->setTextInteractionFlags(Qt::TextSelectableByMouse);
    connect(ui->update_button, &QPushButton::clicked, this, &card_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &card_widget::onDeleteClicked);
}

card_widget::~card_widget() {
    delete ui;
}

void card_widget::onUpdateClicked() {
    emit updateRequested(card_id);
}
void card_widget::onDeleteClicked() {
    emit deleteRequested(card_id);
}