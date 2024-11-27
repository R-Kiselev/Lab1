//
// Created by Waimale on 11/27/2024.
//

#ifndef COURSE_WORK_REPORTS_H
#define COURSE_WORK_REPORTS_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class reports; }
QT_END_NAMESPACE

class reports : public QWidget {
    Q_OBJECT

public:
    explicit reports(QWidget *parent = nullptr);

    ~reports() override;

private:
    Ui::reports *ui;
};


#endif //COURSE_WORK_REPORTS_H
