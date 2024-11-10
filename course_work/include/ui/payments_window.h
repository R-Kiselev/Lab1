//
// Created by Waimale on 11/8/2024.
//

#ifndef COURSE_WORK_PAYMENTS_WINDOW_H
#define COURSE_WORK_PAYMENTS_WINDOW_H

#include <QWidget>
#include "transaction_dialog.h"
#include "../../sqlite/sqlite3.h"

QT_BEGIN_NAMESPACE
namespace Ui { class payments_window; }
QT_END_NAMESPACE

class payments_window : public QWidget {
Q_OBJECT

public:
    explicit payments_window(sqlite3* db);

    ~payments_window() override;
signals:
    void back_button();
private slots:
    void go_back();
    void open_transaction_dialog(int transfer_type);
private:
    std::unique_ptr<Ui::payments_window> ui;
    transaction_dialog* transaction_dialog_;
    sqlite3* db_;
};


#endif //COURSE_WORK_PAYMENTS_WINDOW_H
