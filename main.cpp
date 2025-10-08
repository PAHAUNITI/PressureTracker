#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QLibraryInfo>
#include "mainwindow.h"
#include "authdialog.h"
#include "translations.h"  // Добавляем наш менеджер переводов

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Настройка организации и приложения
    QCoreApplication::setOrganizationName("PressureTracker");
    QCoreApplication::setApplicationName("PressureTracker");

    // Инициализация нашей системы переводов ДО создания окон
    QSettings settings;
    QString language = settings.value("language", "en").toString();
    TranslationManager::instance().setLanguage(language);

    // УДАЛЕНО: Старый код с QTranslator (больше не нужен)
    // QTranslator translator;
    // QTranslator qtTranslator;

    // if (language == "ru") {
    //     // Загрузка переводов Qt (упрощенная версия)
    //     qtTranslator.load("qt_ru", ":/translations/");
    //
    //     // Загрузка переводов приложения
    //     if (translator.load(":/translations/pressuretracker_ru.qm")) {
    //         app.installTranslator(&translator);
    //     }
    //     if (qtTranslator.load("qt_ru", ":/translations/")) {
    //         app.installTranslator(&qtTranslator);
    //     }
    // }

    // Проверка авторизации
    bool isLoggedIn = settings.value("isLoggedIn", false).toBool();

    if (!isLoggedIn) {
        AuthDialog authDialog;
        if (authDialog.exec() == QDialog::Accepted) {
            MainWindow window;
            window.show();
            return app.exec();
        }
    } else {
        MainWindow window;
        window.show();
        return app.exec();
    }

    return 0;
}
