//
// Created by Waimale on 11/18/2024.
//

#ifndef COURSE_WORK_REGISTRATION_WINDOW_H
#define COURSE_WORK_REGISTRATION_WINDOW_H

#include <QWidget>
#include "../../sqlite/sqlite3.h"
#include "memory"
#include "QMessageBox"
#include "../SocialStatus/SocialStatusService.h"
#include "../Client/ClientService.h"
#include "client_dialog.h"
#include "../validation_utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class registration_window; }
QT_END_NAMESPACE

class registration_window : public QWidget {
Q_OBJECT

public:
    explicit registration_window(QWidget *parent = nullptr, sqlite3* db = nullptr);

    ~registration_window() override;

    void setup_services();
signals:
    void back_button();
private slots:
    void handle_registration();
    void handle_add_client();
    void handle_back();
    void update_register_button_state();
private:
    Ui::registration_window *ui;
    sqlite3* db_;

    std::unique_ptr<SocialStatusRepository> social_status_repository;
    std::unique_ptr<SocialStatusService> social_status_service;

    std::unique_ptr<ClientRepository> client_repository;
    std::unique_ptr<ClientService> client_service;

    std::unique_ptr<Client> client_;
    std::unique_ptr<ValidationService> validation_service_;
};


#endif //COURSE_WORK_REGISTRATION_WINDOW_H
