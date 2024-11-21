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

    connect(ui->exit_button, &QPushButton::clicked, this, &mainwindow::close);
}
mainwindow::~mainwindow() {
    if(db_){
        sqlite3_close(db_);
        db_ = nullptr;
    }
}