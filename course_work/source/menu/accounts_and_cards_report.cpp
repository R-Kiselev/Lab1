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
bool accounts_and_cards_report::isDateValid(const std::string& date, const std::string& minDate, const std::string& maxDate) {
    auto parse = [](const std::string& d) {
        int month = std::stoi(d.substr(0, 2));
        int year = std::stoi(d.substr(3, 2));
        return year * 100 + month; // Формат YYYYMM
    };

    int parsedDate = parse(date);
    int parsedMin = minDate.empty() ? 0 : parse(minDate); // Минимальная граница
    int parsedMax = maxDate.empty() ? 999912 : parse(maxDate); // Максимальная граница

    return parsedDate >= parsedMin && parsedDate <= parsedMax;
}

void accounts_and_cards_report::generateReport() {
    QString IBANFilter = ui->lineEdit_IBAN->text();
    int minAccountBalance = ui->spinBox_accountMinBalance->value();
    int maxAccountBalance = ui->spinBox_accountMaxBalance->value();
    int minCardBalance = ui->spinBox_cardMinBalance->value();
    int maxCardBalance = ui->spinBox_cardMaxBalance->value();
    QString minExpireDate = ui->lineEdit_minExpireDate->text();
    QString maxExpireDate = ui->lineEdit_maxExpireDate->text();

    std::string query = R"(
        SELECT accounts.IBAN, accounts.balance, cards.number, cards.expire_date, cards.balance
        FROM accounts
        LEFT JOIN cards ON accounts.id = cards.account_id
        WHERE accounts.bank_id = ?
        AND accounts.balance BETWEEN ? AND ?
        AND (cards.balance BETWEEN ? AND ? OR cards.balance IS NULL)
    )";

    if (!IBANFilter.isEmpty()) {
        query += " AND accounts.IBAN LIKE ?";
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int bindIndex = 1;
        sqlite3_bind_int(stmt, bindIndex++, selectedBankId_);
        sqlite3_bind_int(stmt, bindIndex++, minAccountBalance);
        sqlite3_bind_int(stmt, bindIndex++, maxAccountBalance);
        sqlite3_bind_int(stmt, bindIndex++, minCardBalance);
        sqlite3_bind_int(stmt, bindIndex++, maxCardBalance);

        if (!IBANFilter.isEmpty()) {
            std::string likeIBAN = "%" + IBANFilter.toStdString() + "%";
            sqlite3_bind_text(stmt, bindIndex++, likeIBAN.c_str(), -1, SQLITE_TRANSIENT);
        }

        std::unordered_map<std::string, nlohmann::json> accounts;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string IBAN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int accountBalance = sqlite3_column_int(stmt, 1);
            const char* cardNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* expireDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int cardBalance = sqlite3_column_type(stmt, 4) != SQLITE_NULL ? sqlite3_column_int(stmt, 4) : 0;

            // Фильтрация карт по дате истечения
            if (cardNumber && (!minExpireDate.isEmpty() || !maxExpireDate.isEmpty())) {
                if (!isDateValid(expireDate, minExpireDate.toStdString(), maxExpireDate.toStdString())) {
                    continue; // Пропустить карту, если она не попадает в диапазон
                }
            }

            // Если аккаунт еще не добавлен, добавляем
            if (accounts.find(IBAN) == accounts.end()) {
                accounts[IBAN] = {
                        {"IBAN", IBAN},
                        {"account_balance", accountBalance},
                        {"cards", nlohmann::json::array()}
                };
            }

            // Добавляем информацию о карте
            if (cardNumber) {
                accounts[IBAN]["cards"].push_back({
                                                          {"card_number", cardNumber},
                                                          {"expire_date", expireDate ? expireDate : ""},
                                                          {"card_balance", cardBalance}
                                                  });
            }
        }

        // Преобразуем результат в массив
        currentReport = nlohmann::json::array();
        for (const auto& [_, account] : accounts) {
            currentReport.push_back(account);
        }

        sqlite3_finalize(stmt);
        QMessageBox::information(this, "Успех", "Отчет успешно сгенерирован.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось выполнить запрос.");
    }
}




void accounts_and_cards_report::saveReport() {
    if (currentReport.empty()) {
        QMessageBox::warning(this, "Error", "Report is empty.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save report", "D:\\Study\\2\\PHLL\\course_work\\Reports\\accounts_and_cards_report.json", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        std::ofstream outFile(filePath.toStdString());
        outFile << currentReport.dump(4);
        outFile.close();

        QMessageBox::information(this, "Success", "Report successfully saved.");
    }
}
