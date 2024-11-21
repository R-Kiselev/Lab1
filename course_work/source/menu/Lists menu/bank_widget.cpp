//
// Created by Waimale on 10/25/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_bank_widget.h" resolved

#include "../../../include/ui/bank_widget.h"
#include "ui_bank_widget.h"
#include <QMouseEvent>

bank_widget::bank_widget(const Bank* bank, QWidget *parent, bool is_admin)
        : QWidget(parent), ui(new Ui::bank_widget),  bank(bank), is_admin_(is_admin)
{
    setBankId(bank->get_id());
    ui->setupUi(this);
    ui->labelId->setText("Id: " + QString::number(bank->get_id()));
    ui->labelName->setText("Name: " + QString::fromStdString(bank->get_name()));

    if (is_admin == true){
        connect(ui->update_button, &QPushButton::clicked, this, &bank_widget::onUpdateClicked);
        connect(ui->delete_button, &QPushButton::clicked, this, &bank_widget::onDeleteClicked);
    }
    else {
        ui->update_button->hide();
        ui->delete_button->hide();
    }
}

bank_widget::~bank_widget() = default;

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

void bank_widget::setBankId(int bankId) {
    bank_id = bankId;
}
