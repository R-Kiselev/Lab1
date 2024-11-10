//
// Created by Waimale on 10/28/2024.
//

#ifndef COURSE_WORK_CLIENT_WIDGET_H
#define COURSE_WORK_CLIENT_WIDGET_H

#include <QWidget>
#include "../Client/Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class client_widget; }
QT_END_NAMESPACE

class client_widget : public QWidget {
    Q_OBJECT

public:
    explicit client_widget(const Client* client, QWidget *parent = nullptr);
    ~client_widget() override;

signals:
    void clicked(int client_id);
    void updateRequested(int client_id);
    void deleteRequested(int client_id);
private slots:
    void mousePressEvent(QMouseEvent* event) override;
    void onUpdateClicked();
    void onDeleteClicked();
private:
    std::unique_ptr<Ui::client_widget> ui;
    int client_id;
    Client* client;
};


#endif //COURSE_WORK_CLIENT_WIDGET_H
