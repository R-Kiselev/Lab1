//
// Created by Waimale on 11/5/2024.
//

#ifndef COURSE_WORK_ACCOUNT_DIALOG_H
#define COURSE_WORK_ACCOUNT_DIALOG_H

#include <QDialog>
#include "../Account/AccountService.h"

QT_BEGIN_NAMESPACE
namespace Ui { class account_dialog; }
QT_END_NAMESPACE

class account_dialog : public QDialog {
Q_OBJECT

public:
    explicit account_dialog(AccountService* account_service, QWidget *parent = nullptr);
    ~account_dialog() override;

    std::unique_ptr<Account> get_account() const;
private:
    Ui::account_dialog *ui;
    AccountService* account_service;
    int account_id;
};


#endif //COURSE_WORK_ACCOUNT_DIALOG_H
