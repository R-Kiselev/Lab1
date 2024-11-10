//
// Created by Waimale on 11/5/2024.
//

#ifndef COURSE_WORK_CLIENT_DIALOG_H
#define COURSE_WORK_CLIENT_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "../SocialStatus/SocialStatusService.h"

QT_BEGIN_NAMESPACE
namespace Ui { class client_dialog; }
QT_END_NAMESPACE

class client_dialog : public QDialog {
Q_OBJECT

public:
    explicit client_dialog(SocialStatusService* social_status_service, QWidget *parent = nullptr);
    ~client_dialog() override;

    QString get_name() const;
    int get_social_status_id() const;

private:
    std::unique_ptr<Ui::client_dialog> ui;
    QLineEdit *name_edit;
    QComboBox *social_status_id_edit;
    QPushButton *ok_button;
    QPushButton *cancel_button;
};


#endif //COURSE_WORK_CLIENT_DIALOG_H
