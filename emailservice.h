// emailservice.h
#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include <QString>
#include <QSettings>

class EmailService
{
private:
    EmailService();

    QString smtpServer;
    int smtpPort;
    QString smtpUsername;
    QString smtpPassword;
    bool useSSL;

public:
    // Удаляем копирование и присваивание
    EmailService(const EmailService&) = delete;
    EmailService& operator=(const EmailService&) = delete;

    static EmailService& instance();

    void setSmtpSettings(const QString &server, int port, const QString &username,
                         const QString &password, bool useSSL);

    bool sendPasswordResetEmail(const QString &email, const QString &resetCode);
    bool sendSupportEmail(const QString &fromEmail, const QString &subject, const QString &message);
    bool sendEmail(const QString &to, const QString &subject, const QString &body);

private:
    bool sendEmailWithCurl(const QString &to, const QString &subject, const QString &body);
    bool sendEmailWithPowerShell(const QString &to, const QString &subject, const QString &body);
    bool sendEmailWithMailCommand(const QString &to, const QString &subject, const QString &body);
    bool saveEmailToFile(const QString &to, const QString &subject, const QString &body);
};

#endif // EMAILSERVICE_H
