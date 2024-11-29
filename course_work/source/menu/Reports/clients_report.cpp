#include "../../../include/ui/clients_report.h"
#include "ui_clients_report.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>

using json = nlohmann::json;

clients_report::clients_report(sqlite3* db, int bankId, QWidget *parent) :
        QWidget(parent), ui(new Ui::clients_report), db_(db), bankId_(bankId) {
    ui->setupUi(this);

    populateSocialStatuses();

    connect(ui->generateButton, &QPushButton::clicked, this, &clients_report::generateReport);

    connect(ui->saveButton, &QPushButton::clicked, this, &clients_report::saveReport);

    connect(ui->backButton, &QPushButton::clicked, this, [this](){
        this->close();
        emit back_button_clicked();
    });
}

clients_report::~clients_report() {
    delete ui;
}

void clients_report::populateSocialStatuses() {
    const char* query = "SELECT id, name FROM social_statuses";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) == SQLITE_OK) {
        ui->socialStatusComboBox->addItem("Any", 0);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            QString name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            ui->socialStatusComboBox->addItem(name, id);
        }
        sqlite3_finalize(stmt);
    } else {
        QMessageBox::critical(this, "Error", "Failed to load social statuses.");
    }
}

void clients_report::generateReport() {
    QString nameFilter = ui->nameLineEdit->text();
    QString usernameFilter = ui->usernameLineEdit->text();
    int socialStatusFilter = ui->socialStatusComboBox->currentData().toInt();

    std::string query = "SELECT clients.id, clients.name, social_statuses.name AS social_status_name, clients.username, clients.is_admin "
                        "FROM clients "
                        "JOIN accounts ON clients.id = accounts.client_id "
                        "JOIN social_statuses ON clients.social_status_id = social_statuses.id "
                        "WHERE accounts.bank_id = ?";

    if (!nameFilter.isEmpty()) {
        query += " AND clients.name LIKE ?";
    }
    if (!usernameFilter.isEmpty()) {
        query += " AND clients.username LIKE ?";
    }
    if (socialStatusFilter > 0) {
        query += " AND clients.social_status_id = ?";
    }

    query += "GROUP BY clients.id;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, bankId_);

        int bindIndex = 2;
        if (!nameFilter.isEmpty()) {
            sqlite3_bind_text(stmt, bindIndex++, ("%" + nameFilter.toStdString() + "%").c_str(), -1, SQLITE_TRANSIENT);
        }
        if (!usernameFilter.isEmpty()) {
            sqlite3_bind_text(stmt, bindIndex++, ("%" + usernameFilter.toStdString() + "%").c_str(), -1, SQLITE_TRANSIENT);
        }
        if (socialStatusFilter > 0) {
            sqlite3_bind_int(stmt, bindIndex, socialStatusFilter);
        }

        currentReport = json::array();

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string socialStatusName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            bool isAdmin = sqlite3_column_int(stmt, 4) == 1;

            currentReport.push_back({
                                            {"id", id},
                                            {"name", name},
                                            {"username", username},
                                            {"social_status_name", socialStatusName},
                                            {"is_admin", isAdmin}
                                    });
        }

        sqlite3_finalize(stmt);

        QMessageBox::information(this, "Success", "Report generated successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to execute the query.");
    }
}

void clients_report::saveReport() {
    if (currentReport.empty()) {
        QMessageBox::warning(this, "Error", "Report is empty.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save report", "D:\\Study\\2\\PHLL\\course_work\\Reports\\clients_report.json", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        std::ofstream outFile(filePath.toStdString());
        outFile << currentReport.dump(4);
        outFile.close();

        QMessageBox::information(this, "Success", "Report successfully saved.");
    }
}
