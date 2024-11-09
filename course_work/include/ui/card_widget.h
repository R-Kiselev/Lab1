//
// Created by Waimale on 10/30/2024.
//
#pragma once
#include <QWidget>
#include "../Card/Card.h"

QT_BEGIN_NAMESPACE
namespace Ui { class card_widget; }
QT_END_NAMESPACE

class card_widget : public QWidget {
    Q_OBJECT

public:
    explicit card_widget(QWidget *parent = nullptr, Card* card = nullptr);

    ~card_widget() override;

signals:
    void updateRequested(int card_id);
    void deleteRequested(int card_id);

protected slots:
    void onUpdateClicked();
    void onDeleteClicked();
private:
    Ui::card_widget *ui;
    Card* card;
    int card_id;
};