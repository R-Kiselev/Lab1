//
// Created by Waimale on 11/18/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_registration_window.h" resolved

#include "../../../include/ui/registration_window.h"
#include "ui_registration_window.h"


registration_window::registration_window(QWidget *parent, sqlite3* db) :
        QWidget(parent), ui(new Ui::registration_window), db_(db) {
    ui->setupUi(this);

    setup_services();

    connect(ui->register_button, &QPushButton::clicked, this, &registration_window::handle_registration);
    connect(ui->back_button, &QPushButton::clicked, this, &registration_window::handle_back);

    validation_service_ = std::make_unique<ValidationService>();

    connect(ui->lineUsername, &QLineEdit::textChanged, this, &registration_window::update_register_button_state);
    connect(ui->linePassword, &QLineEdit::textChanged, this, &registration_window::update_register_button_state);

    update_register_button_state();
}

registration_window::~registration_window() {
    delete ui;
}
void registration_window:: update_register_button_state(){
    bool isUsernameEmpty = ui->lineUsername->text().isEmpty();
    bool isPasswordEmpty = ui->linePassword->text().isEmpty();

    ui->register_button->setEnabled(!isUsernameEmpty && !isPasswordEmpty);
}
void registration_window::handle_back() {
    this->close();
    emit back_button();
}
void registration_window::setup_services() {
    social_status_repository = std::make_unique<SocialStatusRepository>(db_);
    social_status_service = std::make_unique<SocialStatusService>(social_status_repository.get());

    client_repository = std::make_unique<ClientRepository>(db_);
    client_service = std::make_unique<ClientService>(client_repository.get(), social_status_service.get());
}
void registration_window::handle_add_client() {
    client_dialog dialog(social_status_service.get(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString client_name = dialog.get_name();
        int social_status_id = dialog.get_social_status_id();

        try{
            client_->set_name(client_name.toStdString());
            client_->set_social_status_id(social_status_id);

            client_service->add(client_.get());
            QMessageBox::information(this, "Registration", "Registration successful");
            handle_back();
        }
        catch (const CustomException& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

}

void registration_window::handle_registration() {
    std::string username = ui->lineUsername->text().toStdString();
    std::string password = ui->linePassword->text().toStdString();
    try{
        validation_service_->validate_username(username);
    }
    catch (const ValidationException& e) {
        QMessageBox::critical(this, "Error", e.what());
        return;
    }

    client_ = std::make_unique<Client>();
    client_->set_username(username);
    client_->set_password(password);

    handle_add_client();
}