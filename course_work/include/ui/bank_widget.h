#pragma once

#include <QWidget>
#include "../Bank/Bank.h"
#include "../session_info.h"

QT_BEGIN_NAMESPACE
namespace Ui { class bank_widget; }
QT_END_NAMESPACE

class bank_widget : public QWidget {
    Q_OBJECT

public:
    explicit bank_widget(const Bank* bank, QWidget *parent = nullptr, bool is_admin = false);
    ~bank_widget() override;

signals:
    void clicked(int bank_id);
    void updateRequested(int bank_id);
    void deleteRequested(int bank_id);

private slots:
    void mousePressEvent(QMouseEvent* event) override;
    void onUpdateClicked();
    void onDeleteClicked();
private:
    std::unique_ptr<Ui::bank_widget> ui;
    int bank_id;
    bool is_admin_;
public:
    void setBankId(int bankId);

private:
    const Bank* bank;
};