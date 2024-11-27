#include "../../include/ui/accounts_and_cards_report.h"
#include "ui_accounts_and_cards_report.h"
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>

accounts_and_cards_report::accounts_and_cards_report(sqlite3* db, int selectedBankId, QWidget *parent) :
        QWidget(parent), ui(new Ui::accounts_and_cards_report), db_(db), selectedBankId_(selectedBankId) {
    ui->setupUi(this);

    setAccountBalanceRange();
    setCardBalanceRange();

    connect(ui->backButton, &QPushButton::clicked, this, [this]() {
        this->close();
        emit back_button_clicked();
    });

    connect(ui->generateReportButton, &QPushButton::clicked, this, &accounts_and_cards_report::generateReport);
    connect(ui->saveReportButton, &QPushButton::clicked, this, &accounts_and_cards_report::saveReport);
}

accounts_and_cards_report::~accounts_and_cards_report() {
    delete ui;
}

void accounts_and_cards_report::setAccountBalanceRange() {
    const char* query = "SELECT MIN(balance), MAX(balance) FROM accounts WHERE bank_id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, selectedBankId_);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int minBalance = sqlite3_column_int(stmt, 0);
            int maxBalance = sqlite3_column_int(stmt, 1);

            ui->spinBox_accountMinBalance->setRange(minBalance, maxBalance);
            ui->spinBox_accountMaxBalance->setRange(minBalance, maxBalance);

            ui->spinBox_accountMinBalance->setValue(minBalance);
            ui->spinBox_accountMaxBalance->setValue(maxBalance);
        }
        sqlite3_finalize(stmt);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить диапазон балансов.");
    }
}
void accounts_and_cards_report::setCardBalanceRange() {
    const char* query = R"(
        SELECT MIN(cards.balance), MAX(cards.balance)
        FROM cards
        INNER JOIN accounts ON cards.account_id = accounts.id
        WHERE accounts.bank_id = ?
    )";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, selectedBankId_);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int minBalance = sqlite3_column_int(stmt, 0);
            int maxBalance = sqlite3_column_int(stmt, 1);

            ui->spinBox_cardMinBalance->setRange(minBalance, maxBalance);
            ui->spinBox_cardMaxBalance->setRange(minBalance, maxBalance);

            ui->spinBox_cardMinBalance->setValue(minBalance);
            ui->spinBox_cardMaxBalance->setValue(maxBalance);
        }
        sqlite3_finalize(stmt);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить диапазон балансов карт.");
    }
}
bool accounts_and_cards_report::validateExpireDate(const QString& date) {
    QRegularExpression re(R"(^([0-9]{2})\/([0-9]{2})$)");
    QRegularExpressionMatch match = re.match(date);

    if (!match.hasMatch()) return false;

    int month = match.captured(1).toInt();
    int year = match.captured(2).toInt();
    int currentYear = QDate::currentDate().year() % 100; // последние 2 цифры текущего года

    if (month < 1 || month > 12) return false;
    if (year < currentYear || year > 99) return false;

    return true;
}

void accounts_and_cards_report::generateReport() {
    QString IBANFilter = ui->lineEdit_IBAN->text();
    int minAccountBalance = ui->spinBox_accountMinBalance->value();
    int maxAccountBalance = ui->spinBox_accountMaxBalance->value();
    int minCardBalance = ui->spinBox_cardMinBalance->value();
    int maxCardBalance = ui->spinBox_cardMaxBalance->value();
    QString minExpireDate = ui->lineEdit_minExpireDate->text();
    QString maxExpireDate = ui->lineEdit_maxExpireDate->text();

    // Validate expire date inputs
    if (!minExpireDate.isEmpty() && !validateExpireDate(minExpireDate)) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат минимальной даты истечения. Используйте формат MM/YY.");
        return;
    }
    if (!maxExpireDate.isEmpty() && !validateExpireDate(maxExpireDate)) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат максимальной даты истечения. Используйте формат MM/YY.");
        return;
    }

    std::string query = R"(
        SELECT accounts.IBAN, accounts.balance, cards.number, cards.expire_date, cards.balance
        FROM accounts
        LEFT JOIN cards ON accounts.id = cards.account_id
        WHERE accounts.bank_id = ?
    )";

    if (!IBANFilter.isEmpty()) {
        query += " AND accounts.IBAN LIKE ?";
    }
    query += " AND accounts.balance BETWEEN ? AND ?";
    query += " AND cards.balance BETWEEN ? AND ?";
    if (!minExpireDate.isEmpty() && !maxExpireDate.isEmpty()) {
        query += " AND CAST(REPLACE(cards.expire_date, '/', '') AS INTEGER) BETWEEN ? AND ?";
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int bindIndex = 1;
        sqlite3_bind_int(stmt, bindIndex++, selectedBankId_);

        if (!IBANFilter.isEmpty()) {
            sqlite3_bind_text(stmt, bindIndex++, ("%" + IBANFilter.toStdString() + "%").c_str(), -1, SQLITE_TRANSIENT);
        }
        sqlite3_bind_int(stmt, bindIndex++, minAccountBalance);
        sqlite3_bind_int(stmt, bindIndex++, maxAccountBalance);
        sqlite3_bind_int(stmt, bindIndex++, minCardBalance);
        sqlite3_bind_int(stmt, bindIndex++, maxCardBalance);

        if (!minExpireDate.isEmpty() && !maxExpireDate.isEmpty()) {
            sqlite3_bind_int(stmt, bindIndex++, std::stoi(minExpireDate.remove('/').toStdString()));
            sqlite3_bind_int(stmt, bindIndex++, std::stoi(maxExpireDate.remove('/').toStdString()));
        }

        currentReport = nlohmann::json::array();

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string IBAN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int accountBalance = sqlite3_column_int(stmt, 1);
            std::string cardNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string expireDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int cardBalance = sqlite3_column_int(stmt, 4);

            currentReport.push_back({
                                            {"IBAN", IBAN},
                                            {"account_balance", accountBalance},
                                            {"card_number", cardNumber},
                                            {"expire_date", expireDate},
                                            {"card_balance", cardBalance}
                                    });
        }
        sqlite3_finalize(stmt);

        QMessageBox::information(this, "Успех", "Отчет успешно сгенерирован.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось выполнить запрос.");
    }
}

void accounts_and_cards_report::saveReport() {
    if (currentReport.empty()) {
        QMessageBox::warning(this, "Ошибка", "Отчет пуст.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить отчет", "accounts_and_cards_report.json", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        std::ofstream outFile(filePath.toStdString());
        outFile << currentReport.dump(4);
        outFile.close();

        QMessageBox::information(this, "Успех", "Отчет успешно сохранен.");
    }
}
