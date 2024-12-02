#include <iostream>
#include "../sqlite/sqlite3.h"
#include "../include/Bank/BankService.h"
#include "../include/SocialStatus/SocialStatusService.h"
#include "../include/Client/ClientService.h"

#include "../include/database_tables.h"

#include <QApplication>
#include "../include/ui/mainwindow.h"
#include "../include/ui/authorization.h"

int main(int argc, char *argv[])
{
    system("chcp 1251");
	sqlite3* DB;

	if (sqlite3_open("D:\\Study\\2\\PHLL\\course_work\\bank.DB", &DB) == SQLITE_OK)
	{
		std::cout << "DB is open" << std::endl;
	}
	else
	{
		std::cerr << sqlite3_errmsg(DB) << std::endl;
		return -1;
	}

    create_banks_table(DB);
    create_social_statuses_table(DB);
    create_clients_table(DB);
    create_transactions_history_table(DB);
	create_accounts_table(DB);
	create_cards_table(DB);

    QApplication a(argc, argv);
    authorization w(nullptr, DB);
    w.show();

    return QApplication::exec();
}
