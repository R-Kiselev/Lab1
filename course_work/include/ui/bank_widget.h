#pragma once

#include <QWidget>
#include "../Bank/Bank.h"

QT_BEGIN_NAMESPACE
namespace Ui { class bank_widget; }
QT_END_NAMESPACE

class bank_widget : public QWidget {
    Q_OBJECT

public:
    explicit bank_widget(const Bank* bank, QWidget *parent = nullptr);
    ~bank_widget() override;

signals:
    void clicked(int bank_id);  // сигнал для передачи id банка
    void updateRequested(int bank_id);  // сигнал для запроса обновления банка
    void deleteRequested(int bank_id);  // сигнал для запроса удаления банка

private slots:
    void mousePressEvent(QMouseEvent* event) override;  // обработка клика
    void onUpdateClicked();
    void onDeleteClicked();
private:
    Ui::bank_widget *ui;
    int bank_id;  // id банка
    const Bank* bank;
};