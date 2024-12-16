cmake_minimum_required(VERSION 3.29)
project(course_work)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Programming/QT/6.8.0/mingw_64")

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "D:/Programming/vcpkg-master/vcpkg-master/scripts/buildsystems/vcpkg.cmake")
endif ()
set(nlohmann_json_DIR "D:/Programming/vcpkg-master/vcpkg-master/installed/x64-windows/share/nlohmann_json")
find_package(nlohmann_json CONFIG REQUIRED)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(course_work
        sqlite/sqlite3.h
        sqlite/sqlite3.c
        include/Errors/CustomExceptions.h
        source/main.cpp
        include/Bank/Bank.h
        source/Bank/Bank.cpp
        include/Bank/BankRepository.h
        source/Bank/BankRepository.cpp
        include/Bank/BankService.h
        source/Bank/BankService.cpp
        include/SocialStatus/SocialStatus.h
        source/SocialStatus/SocialStatus.cpp
        include/SocialStatus/SocialStatusRepository.h
        source/SocialStatus/SocialStatusRepository.cpp
        include/SocialStatus/SocialStatusService.h
        source/SocialStatus/SocialStatusService.cpp
        include/Client/Client.h
        source/Client/Client.cpp
        include/Client/ClientRepository.h
        source/Client/ClientRepository.cpp
        include/Client/ClientService.h
        source/Client/ClientService.cpp
        include/Account/Account.h
        source/Account/Account.cpp
        include/Account/AccountRepository.h
        source/Account/AccountRepository.cpp
        include/Account/AccountService.h
        source/Account/AccountService.cpp
        include/Card/Card.h
        source/Card/Card.cpp
        include/Card/CardRepository.h
        source/Card/CardRepository.cpp
        include/Card/CardService.h
        source/Card/CardService.cpp
        "source/menu/mainwindow.cpp"
        include/ui/mainwindow.h
        "source/menu/mainwindow.ui"
        "source/menu/Lists menu/bank_widget.cpp"
        include/ui/bank_widget.h
        "source/menu/Lists menu/bank_widget.ui"
        "source/menu/Lists menu/accounts_window.cpp"
        include/ui/accounts_window.h
        "source/menu/Lists menu/accounts_window.ui"
        "source/menu/Lists menu/clients_window.cpp"
        include/ui/clients_window.h
        "source/menu/Lists menu/clients_window.ui"
        "source/menu/Lists menu/client_widget.cpp"
        include/ui/client_widget.h
        "source/menu/Lists menu/client_widget.ui"
        "source/menu/Lists menu/account_widget.cpp"
        include/ui/account_widget.h
        "source/menu/Lists menu/account_widget.ui"
        "source/menu/Lists menu/cards_window.cpp"
        include/ui/cards_window.h
        "source/menu/Lists menu/cards_window.ui"
        "source/menu/Lists menu/card_widget.cpp"
        include/ui/card_widget.h
        "source/menu/Lists menu/card_widget.ui"
        "source/menu/Lists menu/bank_window.cpp"
        include/ui/bank_window.h
        "source/menu/Lists menu/bank_window.ui"
        "source/menu/Lists menu/client_dialog.cpp"
        include/ui/client_dialog.h
        "source/menu/Lists menu/client_dialog.ui"
        source/menu/Payments/payments_window.cpp
        include/ui/payments_window.h
        source/menu/Payments/payments_window.ui
        source/menu/Payments/transaction_dialog.cpp
        include/ui/transaction_dialog.h
        source/menu/Payments/transaction_dialog.ui
        include/Transaction/Transaction.h
        include/Transaction/TransactionService.h
        include/interfaces.h
        include/database_tables.h
        include/validation_utils.h
        "source/menu/Auth and registration/registration_window.cpp"
        include/ui/registration_window.h
        "source/menu/Auth and registration/registration_window.ui"
        "source/menu/Auth and registration/authorization.cpp"
        include/ui/authorization.h
        "source/menu/Auth and registration/authorization.ui"
        source/menu/Reports/reports.cpp
        include/ui/reports.h
        source/menu/Reports/reports.ui
        source/menu/Reports/clients_report.cpp
        include/ui/clients_report.h
        source/menu/Reports/clients_report.ui
        source/menu/Reports/accounts_and_cards_report.cpp
        include/ui/accounts_and_cards_report.h
        source/menu/Reports/accounts_and_cards_report.ui
        include/list.h
)
target_link_libraries(course_work
        Qt::Core
        Qt::Gui
        Qt::Widgets
        nlohmann_json::nlohmann_json
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
