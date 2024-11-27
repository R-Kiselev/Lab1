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
#include "reports.h"
QT_BEGIN_NAMESPACE
namespace Ui {
    class mainwindow;
}
QT_END_NAMESPACE

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr, sqlite3* db = nullptr, int user_id = -1);
    ~mainwindow() override;

    sqlite3* open_database(const std::string& db_path);
    bool get_is_admin(int user_id) const;
signals:
    void logout();
private:
    std::unique_ptr<Ui::mainwindow> ui;
    QVBoxLayout *layout;
    std::unique_ptr<payments_window> payments_window_;
    std::unique_ptr<bank_window> bank_window_;
    std::unique_ptr<reports> reports_window_;
    sqlite3* db_;

    int user_id_;
    bool is_admin_;
};
