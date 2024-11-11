//
// Created by Waimale on 10/28/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_client_widget.h" resolved

#include "../../include/ui/client_widget.h"
#include "ui_client_widget.h"
#include <QMouseEvent>

client_widget::client_widget(const Client* client, QWidget *parent) :
        QWidget(parent), ui(new Ui::client_widget), client_id(client ? client->get_id() : -1)
        {
    ui->setupUi(this);
    ui->labelId->setText("Id: " + QString::number(client->get_id()));
    ui->labelName->setText("Name: " + QString::fromStdString(client->get_name()));
    ui->labelSocialStatusId->setText("Social status id: " + QString::number(client->get_social_status_id()));

    connect(ui->update_button, &QPushButton::clicked, this, &client_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &client_widget::onDeleteClicked);
}

client_widget::~client_widget() = default;

void client_widget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(client_id);
    }
}

void client_widget::onUpdateClicked() {
    emit updateRequested(client_id);
}

void client_widget::onDeleteClicked() {
    emit deleteRequested(client_id);
}