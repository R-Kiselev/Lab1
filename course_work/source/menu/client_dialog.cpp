//
// Created by Waimale on 11/5/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_client_dialog.h" resolved

#include "../../include/ui/client_dialog.h"
#include "ui_client_dialog.h"

client_dialog::client_dialog(SocialStatusService* social_status_service, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::client_dialog)
{
    ui->setupUi(this);

    // Подключаем кнопки к диалоговым слотам
    connect(ui->ok_button, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel_button, &QPushButton::clicked, this, &QDialog::reject);

    // Заполняем QComboBox списком социальных статусов
    auto statuses = social_status_service->get_all();
    for (const auto& status : statuses) {
        ui->social_status_combo->addItem(QString::fromStdString(status->get_name()), status->get_id());
    }
}

client_dialog::~client_dialog() {
}

QString client_dialog::get_name() const {
    return ui->name_edit->text();
}

int client_dialog::get_social_status_id() const {
    return ui->social_status_combo->currentData().toInt();
}