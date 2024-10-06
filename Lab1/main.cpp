#include <sqlite3.h>
#include "database_tables.h"
#include "menu_account_card.h"
#include "BankCard.h"
#include "Account.h"

/*
0. ��������� �������
1. ������ ���� ID �������
2. ������� ������� ��� 2 ���� - ����������, �������� ��������� � ����, ������� �������� �� ��, �������������� ���� ��� �����\��������
3. �������� 2 ���������� ���������� ������ [id]
4. �������� ������������� �������
5. ��������� ��� ������ sonar
6. ������� �����
7. �������� ������
������� ��������� � ��������� ����
*/
int main()
{
    sqlite3* DB;
    int rc = sqlite3_open("D:\\Study\\2\\PHLL\\Lab1\\Database\\bank.db", &DB);
    
    if (rc == SQLITE_OK)
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