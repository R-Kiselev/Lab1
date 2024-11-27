//
// Created by Waimale on 11/27/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_reports.h" resolved

#include "reports.h"
#include "ui_reports.h"


reports::reports(QWidget *parent) :
        QWidget(parent), ui(new Ui::reports) {
    ui->setupUi(this);
}

reports::~reports() {
    delete ui;
}
