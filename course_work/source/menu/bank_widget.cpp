//
// Created by Waimale on 10/25/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_bank_widget.h" resolved

#include "../../include/ui/bank_widget.h"
#include "ui_bank_widget.h"
#include <QMouseEvent>

bank_widget::bank_widget(const Bank* bank, QWidget *parent)
        : QWidget(parent), ui(new Ui::bank_widget),  bank(bank)
{
    bank_id = bank ? bank->get_id() : -1;
    ui->setupUi(this);
    ui->labelId->setText("Id: " + QString::number(bank->get_id()));
    ui->labelName->setText("Name: " + QString::fromStdString(bank->get_name()));

    connect(ui->update_button, &QPushButton::clicked, this, &bank_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &bank_widget::onDeleteClicked);
}

bank_widget::~bank_widget() {
}

void bank_widget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(bank_id);
    }
}

void bank_widget::onUpdateClicked() {
    emit updateRequested(bank_id);
}

void bank_widget::onDeleteClicked() {
    emit deleteRequested(bank_id);
}