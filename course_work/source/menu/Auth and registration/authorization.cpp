//
// Created by Waimale on 11/20/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_authorization.h" resolved

#include "../../../include/ui/authorization.h"
#include "ui_authorization.h"


authorization::authorization(QWidget *parent, sqlite3* db) :
        QWidget(parent), ui(new Ui::authorization), db_(db) {
    ui->setupUi(this);
    setup_services();

    connect(ui->sign_in_button, &QPushButton::clicked, this, &authorization::handle_sign_in);
    connect(ui->register_button, &QPushButton::clicked, this, &authorization::handle_registration);
    registration_window_ = std::make_unique<registration_window>(nullptr, db_);
    connect(registration_window_.get(), &registration_window::back_button, this, &authorization::show);

    connect(ui->lineEditUsername, &QLineEdit::textChanged, this, &authorization::update_login_button_state);
    connect(ui->lineEditPassword, &QLineEdit::textChanged, this, &authorization::update_login_button_state);
    connect(ui->im_not_Misha_check, &QCheckBox::toggled, this, &authorization::update_login_button_state);

    update_login_button_state();

    connect(ui->im_not_Misha_check, &QCheckBox::toggled, this, &authorization::update_register_button_state);
    update_register_button_state();
}

authorization::~authorization() {
    if (db_) {
        sqlite3_close(db_);
    }
    delete ui;
}
void authorization::update_register_button_state(){
    bool isCheckBoxUnchecked = !ui->im_not_Misha_check->isChecked();

    ui->register_button->setEnabled(!isCheckBoxUnchecked);

}
void authorization::update_login_button_state() {
    bool isUsernameEmpty = ui->lineEditUsername->text().isEmpty();
    bool isPasswordEmpty = ui->lineEditPassword->text().isEmpty();
    bool isCheckBoxUnchecked = !ui->im_not_Misha_check->isChecked();

    ui->sign_in_button->setEnabled(!isUsernameEmpty && !isPasswordEmpty && !isCheckBoxUnchecked);
}

void authorization::setup_services() {
    social_status_repository = std::make_unique<SocialStatusRepository>(db_);
    social_status_service = std::make_unique<SocialStatusService>(social_status_repository.get());
    client_repository = std::make_unique<ClientRepository>(db_);
    client_service = std::make_unique<ClientService>(client_repository.get(), social_status_service.get());
}

std::unique_ptr<Client> authorization::get_client_by_username_and_password(const QString& username, const QString& password) {
    std::string sql = std::format(
            "SELECT * FROM clients "
                 "WHERE username = '{}' AND password = '{}'",
                 username.toStdString(), password.toStdString());

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare SQL statement for getting client by ID");
    }

    if(sqlite3_step(stmt) != SQLITE_ROW){
        sqlite3_finalize(stmt);
        throw NotFoundException(std::format("Client with username {} not found in database", username.toStdString()));
    }
    int client_id = sqlite3_column_int(stmt, 0);
    std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    int social_status_id = sqlite3_column_int(stmt, 2);
    bool is_admin = sqlite3_column_int(stmt, 3) == 1;

    auto client = std::make_unique<Client>(name, social_status_id);
    client->set_is_admin(is_admin);
    client->set_id(client_id);

    return client;
}

void authorization::handle_sign_in() {
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled out.");
        return;
    }

    try{
        auto client = get_client_by_username_and_password(username, password);
        auto main_window = new mainwindow(nullptr, db_, client->get_id());
        main_window->show();
        connect(main_window, &mainwindow::logout, this, &authorization::show);
        this->close();
    }
    catch (NotFoundException& e){
        QMessageBox::critical(this, "Error", e.what());
    }
}

void authorization::handle_registration() {
    registration_window_->show();
    this->close();
}