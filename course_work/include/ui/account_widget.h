//
// Created by Waimale on 10/30/2024.
//

#ifndef COURSE_WORK_ACCOUNT_WIDGET_H
#define COURSE_WORK_ACCOUNT_WIDGET_H

#include <QWidget>
#include "../Account/Account.h"

QT_BEGIN_NAMESPACE
namespace Ui { class account_widget; }
QT_END_NAMESPACE

class account_widget : public QWidget {
    Q_OBJECT

public:
    explicit account_widget(QWidget *parent = nullptr, Account* account = nullptr);

    ~account_widget() override;
    void set_account_id(int account_id_);
signals:
    void clicked(int account_id);
    void updateRequested(int account_id);
    void deleteRequested(int account_id);
private slots:
    void mousePressEvent(QMouseEvent* event) override;
    void onUpdateClicked();
    void onDeleteClicked();
private:
    std::unique_ptr<Ui::account_widget> ui;
    int account_id;
    Account* account;
};


#endif //COURSE_WORK_ACCOUNT_WIDGET_H
