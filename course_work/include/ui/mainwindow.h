//
// Created by Waimale on 10/25/2024.
//
#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>

#include "bank_window.h"
#include "payments_window.h"
#include "../../sqlite/sqlite3.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
    class mainwindow;
}
QT_END_NAMESPACE

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow() override;

    sqlite3* open_database(const std::string& db_path);
private:
    std::unique_ptr<Ui::mainwindow> ui;
    QVBoxLayout *layout;

    std::unique_ptr<payments_window> payments_window_;
    std::unique_ptr<bank_window> bank_window_;
    sqlite3* db_;
};
