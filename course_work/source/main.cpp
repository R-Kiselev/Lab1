#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <format>
#include "../sqlite/sqlite3.h"
#include "../include/Bank/BankService.h"
#include "../include/SocialStatus/SocialStatusService.h"
#include "../include/Client/ClientService.h"
#include "../include/Account/AccountService.h"
#include "../include/Card/CardService.h"

#include "../include/database_tables.h"

#include <QApplication>
#include "../include/ui/mainwindow.h"
#include "../include/ui/clients_window.h"

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
	create_accounts_table(DB);
	create_cards_table(DB);


////	menu_account_card(DB);
//    BankRepository bank_repo(DB);
//    BankService bank_service(&bank_repo);
//
//    SocialStatusRepository social_status_repo(DB);
//    SocialStatusService social_status_service(&social_status_repo);
//
//    ClientRepository client_repo(DB);
//    ClientService client_service(&client_repo, &social_status_service);
//
//    AccountRepository account_repo(DB);
//    AccountService account_service(&account_repo, &client_service, &bank_service);
//
//    CardRepository card_repo(DB);
//    CardService card_service(&card_repo, &account_service);

    sqlite3_close(DB);

    QApplication a(argc, argv);
    mainwindow w;
    w.show();

    int result = a.exec();
    return result;
}
