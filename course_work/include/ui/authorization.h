//
// Created by Waimale on 11/20/2024.
//

#ifndef COURSE_WORK_AUTHORIZATION_H
#define COURSE_WORK_AUTHORIZATION_H

#include <QWidget>
#include "../../sqlite/sqlite3.h"
#include "memory"
#include "QMessageBox"
#include "mainwindow.h"
#include "registration_window.h"
#include "../SocialStatus/SocialStatusService.h"
#include "../Client/ClientService.h"

QT_BEGIN_NAMESPACE
namespace Ui { class authorization; }
QT_END_NAMESPACE

class authorization : public QWidget {
Q_OBJECT

public:
    explicit authorization(QWidget *parent = nullptr, sqlite3* db = nullptr);

    ~authorization() override;
    void setup_services();
    std::unique_ptr<Client> get_client_by_username_and_password(const QString& username, const QString& password);

private slots:
    void handle_sign_in();
    void handle_registration();
    void update_login_button_state();
    void update_register_button_state();
private:
    Ui::authorization *ui;
    sqlite3* db_;

    std::unique_ptr<registration_window> registration_window_;

    std::unique_ptr<ClientRepository> client_repository;
    std::unique_ptr<ClientService> client_service;
    std::unique_ptr<SocialStatusRepository> social_status_repository;
    std::unique_ptr<SocialStatusService> social_status_service;
};


#endif //COURSE_WORK_AUTHORIZATION_H
