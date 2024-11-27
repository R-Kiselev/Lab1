//
// Created by Waimale on 10/25/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include "../../include/ui/mainwindow.h"
#include "ui_mainwindow.h"

mainwindow::mainwindow(QWidget *parent, sqlite3* db, int user_id)
        : QMainWindow(parent), ui{new Ui::mainwindow}, db_(db), user_id_(user_id)
{
    ui->setupUi(this);
    setWindowTitle("Главное меню");

    is_admin_ = get_is_admin(user_id_);

    bank_window_ = std::make_unique<bank_window>(db_, user_id_);
    connect(bank_window_.get(), &bank_window::back_button, this, &mainwindow::show);
    connect(ui->banks_button, &QPushButton::clicked, this, [this](){
        this->close();
        bank_window_->load_banks();
        bank_window_->show();
    });

    payments_window_ = std::make_unique<payments_window>(db_, user_id_);
    connect(payments_window_.get(), &payments_window::back_button, this, &mainwindow::show);
    connect(ui->payments_button, &QPushButton::clicked, this, [this](){
        this->close();
        payments_window_->show();
    });

    connect(ui->logout_button, &QPushButton::clicked, this, [this](){
        this->close();
        emit logout();
    });

    if (!is_admin_){
        ui->pushButton_reports->hide();
    }
    else{
        reports_window_ = std::make_unique<reports>(db_);
        connect(reports_window_.get(), &reports::back_button_clicked, this, &mainwindow::show);
        connect(ui->pushButton_reports, &QPushButton::clicked, this, [this](){
            this->close();
            reports_window_->show();
        });
    }

    connect(ui->exit_button, &QPushButton::clicked, this, &mainwindow::close);
}
bool mainwindow::get_is_admin(int user_id) const {
    std::string sql = "SELECT is_admin FROM clients WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by ID");
    }
    sqlite3_bind_int(stmt, 1, user_id);
    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("User with ID {} not found in database", std::to_string(user_id)));
    }
    bool is_admin = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return is_admin;
}
mainwindow::~mainwindow() {
    if(db_){
        sqlite3_close(db_);
        db_ = nullptr;
    }
}