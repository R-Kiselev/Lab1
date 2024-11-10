//
// Created by Waimale on 10/25/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include "../../include/ui/mainwindow.h"
#include "ui_mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
        : QMainWindow(parent), ui{new Ui::mainwindow}
{
    ui->setupUi(this);
    setWindowTitle("Главное меню");

    const std::string db_path = "D:\\Study\\2\\PHLL\\course_work\\bank.db";
    db_ = open_database(db_path);

    bank_window_ = std::make_unique<bank_window>(db_);
    connect(bank_window_.get(), &bank_window::back_button, this, &mainwindow::show);
    connect(ui->banks_button, &QPushButton::clicked, this, [this](){
        this->close();
        bank_window_->load_banks();
        bank_window_->show();
    });

    payments_window_ = std::make_unique<payments_window>(db_);
    connect(payments_window_.get(), &payments_window::back_button, this, &mainwindow::show);
    connect(ui->payments_button, &QPushButton::clicked, this, [this](){
        this->close();
        payments_window_->show();
    });

    connect(ui->exit_button, &QPushButton::clicked, this, &mainwindow::close);
}
sqlite3* mainwindow::open_database(const std::string& db_path) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return nullptr;
    }
    return db;
}
mainwindow::~mainwindow() {
    if(db_){
        sqlite3_close(db_);
        db_ = nullptr;
    }
}