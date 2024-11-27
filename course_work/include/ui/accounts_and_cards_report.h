#ifndef COURSE_WORK_ACCOUNTS_AND_CARDS_REPORT_H
#define COURSE_WORK_ACCOUNTS_AND_CARDS_REPORT_H

#include <QWidget>
#include <QRegularExpression>
#include <QDate>
#include "../../sqlite/sqlite3.h"
#include <nlohmann/json.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class accounts_and_cards_report; }
QT_END_NAMESPACE

class accounts_and_cards_report : public QWidget {
Q_OBJECT

public:
    explicit accounts_and_cards_report(sqlite3* db, int selectedBankId, QWidget *parent = nullptr);
    ~accounts_and_cards_report() override;

signals:
    void back_button_clicked();

private:
    Ui::accounts_and_cards_report *ui;
    sqlite3* db_;
    int selectedBankId_;
    nlohmann::json currentReport;

    void setAccountBalanceRange();
    void setCardBalanceRange();
    void generateReport();
    void saveReport();
    bool validateExpireDate(const QString& date);
};

#endif //COURSE_WORK_ACCOUNTS_AND_CARDS_REPORT_H
