QT += core gui charts widgets

CONFIG += c++17

TARGET = PressureTracker
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    authdialog.cpp \
    addentrydialog.cpp \
    accountdialog.cpp \
    supportdialog.cpp \
    translations.cpp

HEADERS += \
    mainwindow.h \
    authdialog.h \
    addentrydialog.h \
    accountdialog.h \
    supportdialog.h \
    datatypes.h \
    translations.h

RESOURCES +=

TRANSLATIONS =

OTHER_FILES +=

# Автоматическое обновление переводов при сборке
update_translations.commands = lupdate $$PWD/pressuretracker.pro
release_translations.commands = lrelease $$PWD/pressuretracker.pro
QMAKE_EXTRA_TARGETS += update_translations release_translations

# Добавляем настройки для Windows
win32 {
    # Для статической сборки Qt
    CONFIG += static
    # Для MinGW
    QMAKE_LFLAGS += -static
}

# Отключаем предкомпилированные заголовки если они есть
CONFIG -= precompile_header

DISTFILES +=



