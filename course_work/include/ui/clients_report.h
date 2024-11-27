#ifndef COURSE_WORK_CLIENTS_REPORT_H
#define COURSE_WORK_CLIENTS_REPORT_H

#include <QWidget>
#include "../../sqlite/sqlite3.h"
#include <nlohmann/json.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class clients_report; }
QT_END_NAMESPACE

class clients_report : public QWidget {
Q_OBJECT

public:
    explicit clients_report(sqlite3* db, int bankId, QWidget *parent = nullptr);

    ~clients_report() override;

signals:
    void back_button_clicked();
private slots:
    void populateSocialStatuses();
    void generateReport();
    void saveReport();

private:
    Ui::clients_report *ui;
    sqlite3* db_;
    int bankId_;
    nlohmann::json currentReport;
};

#endif //COURSE_WORK_CLIENTS_REPORT_H
