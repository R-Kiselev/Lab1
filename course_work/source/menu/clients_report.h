//
// Created by Waimale on 11/27/2024.
//

#ifndef COURSE_WORK_CLIENTS_REPORT_H
#define COURSE_WORK_CLIENTS_REPORT_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class clients_report; }
QT_END_NAMESPACE

class clients_report : public QWidget {
    Q_OBJECT

public:
    explicit clients_report(QWidget *parent = nullptr);

    ~clients_report() override;

private:
    Ui::clients_report *ui;
};


#endif //COURSE_WORK_CLIENTS_REPORT_H
