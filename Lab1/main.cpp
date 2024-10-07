#include <sqlite3.h>
#include "database_tables.h"
#include "menu_account_card.h"
#include "BankCard.h"
#include "Account.h"

int main()
{
	sqlite3* DB;

	if (sqlite3_open("D:\\Study\\2\\PHLL\\Lab1\\Database\\bank.db", &DB) == SQLITE_OK)
	{
		std::cout << "DB is open" << std::endl;
	}
	else
	{
		std::cerr << sqlite3_errmsg(DB) << std::endl;
		return -1;
	}

	create_account_table(DB);
	create_card_table(DB);

	menu_account_card(DB);

	sqlite3_close(DB);
}