//
// Created by Waimale on 10/28/2024.
//

#ifndef COURSE_WORK_CLIENT_WIDGET_H
#define COURSE_WORK_CLIENT_WIDGET_H

#include <QWidget>
#include "../Client/Client.h"
#include "../../sqlite/sqlite3.h"
#include "../Errors/CustomExceptions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class client_widget; }
QT_END_NAMESPACE

class client_widget : public QWidget {
    Q_OBJECT

public:
    explicit client_widget(const Client* client, QWidget *parent = nullptr, sqlite3* db=nullptr);
    ~client_widget() override;

signals:
    void clicked(int client_id);
    void updateRequested(int client_id);
    void deleteRequested(int client_id);
private slots:
    void mousePressEvent(QMouseEvent* event) override;
    void onUpdateClicked();
    void onDeleteClicked();
    std::string get_social_status_name(int social_status_id);
private:
    std::unique_ptr<Ui::client_widget> ui;
    int client_id;
    Client* client;
    sqlite3* db_;
};


#endif //COURSE_WORK_CLIENT_WIDGET_H
