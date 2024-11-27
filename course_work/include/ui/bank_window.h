//
// Created by Waimale on 10/31/2024.
//

#ifndef COURSE_WORK_BANK_WINDOW_H
#define COURSE_WORK_BANK_WINDOW_H

#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>

#include "bank_widget.h"
#include "clients_window.h"
#include "accounts_window.h"

#include "../../sqlite/sqlite3.h"
#include "../Bank/BankService.h"

#include "../Errors/CustomExceptions.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class bank_window; }
QT_END_NAMESPACE

class bank_window : public QWidget {
    Q_OBJECT

public:
    explicit bank_window(sqlite3* db, int user_id);
    ~bank_window() override;

    void setup_services();
    void load_banks();
    bool get_is_admin() const;
signals:
    void back_button();
private slots:
    void go_back();
    void open_clients_window(int bank_id);
    void open_accounts_window(int bank_id);
    void add();
    void update(int bank_id);
    void delete_bank(int bank_id);

private:
    std::unique_ptr<Ui::bank_window>ui;

    std::unique_ptr<clients_window> clients_window_;
    std::unique_ptr<accounts_window> accounts_window_;

    sqlite3* db_;
    std::unique_ptr<BankRepository> bank_repository;
    std::unique_ptr<BankService> bank_service;

    int user_id_;
    bool is_admin_;
};


#endif //COURSE_WORK_BANK_WINDOW_H
