#include <sqlite3.h>
#include "database_tables.h"
#include "menu_account_card.h"
#include "BankCard.h"
#include "Account.h"

/*
0. Пофиксить нейминг
1. Убрать ввод ID вручную
2. Сделать менюшку для 2 лабы - добавление, удаление аккаунтов и карт, создать удаление из БД, редактирование инфы про карты\аккаунты
3. Добавить 2 перегрузки операторов помимо [id]
4. Добавить дружественную функцию
5. Пофиксить все ошибки sonar
6. Сделать отчет
7. Почитать теорию
ВЫНЕСТИ КОЛЛЕКЦИЮ В ОТДЕЛЬНЫЙ ФАЙЛ
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