#include "emailservice.h"
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>
#include <QApplication>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>

EmailService& EmailService::instance()
{
    static EmailService instance;
    return instance;
}

EmailService::EmailService()
    : smtpServer("smtp.gmail.com"), smtpPort(587), useSSL(true)
{
    // Загрузка настроек SMTP
    QSettings settings;
    smtpServer = settings.value("smtp/server", "smtp.gmail.com").toString();
    smtpPort = settings.value("smtp/port", 587).toInt();
    smtpUsername = settings.value("smtp/username", "").toString();
    smtpPassword = settings.value("smtp/password", "").toString();
    useSSL = settings.value("smtp/useSSL", true).toBool();
}

void EmailService::setSmtpSettings(const QString &server, int port, const QString &username,
                                   const QString &password, bool useSSL)
{
    this->smtpServer = server;
    this->smtpPort = port;
    this->smtpUsername = username;
    this->smtpPassword = password;
    this->useSSL = useSSL;

    // Сохранение настроек
    QSettings settings;
    settings.setValue("smtp/server", server);
    settings.setValue("smtp/port", port);
    settings.setValue("smtp/username", username);
    settings.setValue("smtp/password", password);
    settings.setValue("smtp/useSSL", useSSL);
}

bool EmailService::sendPasswordResetEmail(const QString &email, const QString &resetCode)
{
    QString subject = tr("Password Reset Request - PressureTracker");
    QString body = QString(
                       tr("Hello!\n\n"
                          "You have requested to reset your password for PressureTracker.\n\n"
                          "Your password reset code is: %1\n\n"
                          "Please enter this code in the application to reset your password.\n\n"
                          "If you did not request this reset, please ignore this email.\n\n"
                          "Best regards,\n"
                          "PressureTracker Team"))
                       .arg(resetCode);

    return sendEmail(email, subject, body);
}

bool EmailService::sendSupportEmail(const QString &fromEmail, const QString &subject, const QString &message)
{
    QString fullSubject = QString("Support Request from %1: %2").arg(fromEmail, subject);
    QString body = QString(
                       "Support request details:\n\n"
                       "From: %1\n"
                       "Subject: %2\n\n"
                       "Message:\n%3\n\n"
                       "---\n"
                       "This email was sent from PressureTracker application.")
                       .arg(fromEmail, subject, message);

    // Отправляем на фиксированный email поддержки
    return sendEmail("support@pressuretracker.com", fullSubject, body);
}

bool EmailService::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    qDebug() << "Attempting to send email to:" << to;
    qDebug() << "Subject:" << subject;

    // Способ 1: Используем curl (если доступен)
    if (sendEmailWithCurl(to, subject, body)) {
        qDebug() << "Email sent successfully via curl";
        return true;
    }

    // Способ 2: Используем PowerShell (Windows)
    if (sendEmailWithPowerShell(to, subject, body)) {
        qDebug() << "Email sent successfully via PowerShell";
        return true;
    }

    // Способ 3: Используем mail команду (Linux/Mac)
    if (sendEmailWithMailCommand(to, subject, body)) {
        qDebug() << "Email sent successfully via mail command";
        return true;
    }

    qDebug() << "All email methods failed, saving to file";

    // Если ничего не работает, сохраняем в файл для ручной отправки
    return saveEmailToFile(to, subject, body);
}

bool EmailService::sendEmailWithCurl(const QString &to, const QString &subject, const QString &body)
{
    QProcess process;

    // Проверяем доступность curl
    process.start("curl", QStringList() << "--version");
    if (!process.waitForFinished(5000) || process.exitCode() != 0) {
        qDebug() << "curl not available";
        return false;
    }

    // Создаем временный файл для тела письма
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        qDebug() << "Failed to create temp file";
        return false;
    }

    QTextStream out(&tempFile);
    out << body;
    tempFile.close();

    // Формируем команду curl для отправки через SMTP
    QStringList arguments;
    arguments << "-s"
              << "--url" << QString("smtps://%1:%2").arg(smtpServer).arg(smtpPort)
              << "--ssl-reqd"
              << "--mail-from" << smtpUsername
              << "--mail-rcpt" << to
              << "--user" << QString("%1:%2").arg(smtpUsername).arg(smtpPassword)
              << "-T" << tempFile.fileName()
              << "-H" << QString("Subject: %1").arg(subject);

    qDebug() << "Executing curl with arguments:" << arguments;
    process.start("curl", arguments);

    if (!process.waitForFinished(30000)) { // 30 секунд таймаут
        qDebug() << "curl timeout";
        return false;
    }

    bool success = (process.exitCode() == 0);
    qDebug() << "curl exit code:" << process.exitCode() << "success:" << success;
    return success;
}

bool EmailService::sendEmailWithPowerShell(const QString &to, const QString &subject, const QString &body)
{
#ifdef Q_OS_WINDOWS
    QProcess process;

    // Создаем копию body для модификации
    QString modifiedBody = body;
    modifiedBody.replace("\"", "`\"");
    modifiedBody.replace("\n", "`n");

    // Создаем PowerShell скрипт для отправки email
    QString powerShellScript = QString(
                                   "try {\n"
                                   "    $EmailFrom = \"%1\"\n"
                                   "    $EmailTo = \"%2\"\n"
                                   "    $Subject = \"%3\"\n"
                                   "    $Body = \"%4\"\n"
                                   "    $SMTPServer = \"%5\"\n"
                                   "    $SMTPPort = %6\n"
                                   "    $SMTPUser = \"%7\"\n"
                                   "    $SMTPPassword = \"%8\"\n"
                                   "    $SMTPUseSSL = $%9\n"
                                   "    \n"
                                   "    $SecurePassword = ConvertTo-SecureString $SMTPPassword -AsPlainText -Force\n"
                                   "    $Credential = New-Object System.Management.Automation.PSCredential($SMTPUser, $SecurePassword)\n"
                                   "    \n"
                                   "    Send-MailMessage -From $EmailFrom -To $EmailTo -Subject $Subject -Body $Body "
                                   "-SmtpServer $SMTPServer -Port $SMTPPort -UseSsl:$SMTPUseSSL -Credential $Credential\n"
                                   "    exit 0\n"
                                   "} catch {\n"
                                   "    Write-Error $_.Exception.Message\n"
                                   "    exit 1\n"
                                   "}"
                                   ).arg(smtpUsername, to, subject, modifiedBody,
                                        smtpServer, QString::number(smtpPort), smtpUsername, smtpPassword,
                                        useSSL ? "true" : "false");

    QStringList arguments;
    arguments << "-Command" << powerShellScript;

    qDebug() << "Executing PowerShell script";
    process.start("powershell", arguments);

    if (!process.waitForFinished(30000)) {
        qDebug() << "PowerShell timeout";
        return false;
    }

    bool success = (process.exitCode() == 0);
    qDebug() << "PowerShell exit code:" << process.exitCode() << "success:" << success;

    if (!success) {
        qDebug() << "PowerShell error:" << process.readAllStandardError();
    }

    return success;
#else
    qDebug() << "PowerShell not available on non-Windows platform";
    return false;
#endif
}

bool EmailService::sendEmailWithMailCommand(const QString &to, const QString &subject, const QString &body)
{
#ifndef Q_OS_WINDOWS
    QProcess process;

    // Проверяем доступность команды mail
    process.start("which", QStringList() << "mail");
    process.waitForFinished();

    if (process.exitCode() != 0) {
        qDebug() << "mail command not found";
        return false; // Команда mail не найдена
    }

    // Отправляем email через mail команду
    qDebug() << "Executing mail command";
    process.start("mail", QStringList() << "-s" << subject << to);
    process.write(body.toUtf8());
    process.closeWriteChannel();

    if (!process.waitForFinished(30000)) {
        qDebug() << "mail command timeout";
        return false;
    }

    bool success = (process.exitCode() == 0);
    qDebug() << "mail command exit code:" << process.exitCode() << "success:" << success;
    return success;
#else
    qDebug() << "mail command not available on Windows";
    return false;
#endif
}

bool EmailService::saveEmailToFile(const QString &to, const QString &subject, const QString &body)
{
    // Сохраняем email в файл для ручной отправки
    QString emailDir = QApplication::applicationDirPath() + "/emails";
    QDir dir;
    if (!dir.exists(emailDir)) {
        dir.mkpath(emailDir);
    }

    QString filename = QString("%1/email_%2.txt").arg(emailDir).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to save email to file:" << filename;
        return false;
    }

    QTextStream out(&file);
    out << "To: " << to << "\n";
    out << "Subject: " << subject << "\n";
    out << "Date: " << QDateTime::currentDateTime().toString() << "\n";
    out << "From: " << smtpUsername << "\n";
    out << "\n";
    out << body;
    out << "\n\n---\n";
    out << "This email was generated by PressureTracker but could not be sent automatically.\n";
    out << "Please send this email manually or configure SMTP settings in the application.\n";
    out << "SMTP Server: " << smtpServer << ":" << smtpPort << "\n";

    file.close();

    qDebug() << "Email saved to file:" << filename;

    // Показываем сообщение пользователю
    QMessageBox::information(nullptr, tr("Email Saved"),
                             QString(tr("Email could not be sent automatically.\n\n"
                                        "The email has been saved to:\n%1\n\n"
                                        "Please send it manually or check your SMTP settings."))
                                 .arg(filename));

    return true;
}
