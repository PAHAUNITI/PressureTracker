#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <QString>
#include <QMap>
#include <QObject>
#include <QApplication>

class TranslationManager : public QObject
{
    Q_OBJECT

public:
    static TranslationManager& instance();

    // Установка текущего языка
    void setLanguage(const QString& language);
    QString getCurrentLanguage() const { return currentLanguage; }

    // Получение перевода
    QString translate(const QString& key) const;

    // Доступные языки
    QStringList availableLanguages() const;

signals:
    void languageChanged(const QString& newLanguage);

private:
    TranslationManager();
    void loadTranslations();

    QMap<QString, QMap<QString, QString>> translations;
    QString currentLanguage;
};

// Изменяем макрос на уникальное имя чтобы избежать конфликтов
#define TR(key) TranslationManager::instance().translate(key)

#endif // TRANSLATIONS_H
