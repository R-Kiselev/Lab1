//
// Created by Waimale on 10/28/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_client_widget.h" resolved

#include "../../include/ui/client_widget.h"
#include "ui_client_widget.h"
#include <QMouseEvent>

client_widget::client_widget(const Client* client, QWidget *parent, sqlite3* db) :
        QWidget(parent), ui(new Ui::client_widget), client_id(client ? client->get_id() : -1), db_(db)
        {
    ui->setupUi(this);
    ui->labelName->setText("Name: " + QString::fromStdString(client->get_name()));

    std::string social_status = get_social_status_name(client->get_social_status_id());
    ui->labelSocialStatusId->setText("Social status: " + QString::fromStdString(social_status));

    connect(ui->update_button, &QPushButton::clicked, this, &client_widget::onUpdateClicked);
    connect(ui->delete_button, &QPushButton::clicked, this, &client_widget::onDeleteClicked);
}

client_widget::~client_widget() = default;

std::string client_widget::get_social_status_name(int social_status_id){
    std::string sql = std::format("SELECT name FROM social_statuses WHERE id = {};", social_status_id);
    sqlite3_stmt* stmt;
    std::string name;

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException(sqlite3_errmsg(db_));
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Social status with ID {} not found", social_status_id));
    }

    sqlite3_finalize(stmt);
    return name;
}

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