#ifndef COURSE_WORK_REPORTS_H
#define COURSE_WORK_REPORTS_H

#include <QWidget>
#include "../../sqlite/sqlite3.h"
#include "../../include/ui/clients_report.h"
#include "../../include/ui/accounts_and_cards_report.h"

QT_BEGIN_NAMESPACE
namespace Ui { class reports; }
QT_END_NAMESPACE

class reports : public QWidget {
Q_OBJECT

public:
    explicit reports(sqlite3* db, QWidget *parent = nullptr);

    ~reports() override;

signals:
    void back_button_clicked();
private slots:
    void populateBanks();
    void openClientsReport();
    void openAccountsAndCardsReport();

private:
    Ui::reports *ui;
    sqlite3* db_;
    std::unique_ptr<clients_report> clients_report_window_;
    std::unique_ptr<accounts_and_cards_report> accounts_and_cards_report_window_;
};

#endif //COURSE_WORK_REPORTS_H
