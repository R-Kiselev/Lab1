//
// Created by Waimale on 11/27/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_clients_report.h" resolved

#include "clients_report.h"
#include "ui_clients_report.h"


clients_report::clients_report(QWidget *parent) :
        QWidget(parent), ui(new Ui::clients_report) {
    ui->setupUi(this);
}

clients_report::~clients_report() {
    delete ui;
}
