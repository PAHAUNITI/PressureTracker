#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QEvent>
#include "translations.h"  // Добавляем нашу систему переводов

// Forward declarations
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QSettings;
class QFrame;
class QProgressBar;

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    AuthDialog(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void login();
    void registerUser();
    void retranslateUi();
    void validateForm();
    void checkPasswordStrength(const QString &password);
    void togglePasswordVisibility();
    void toggleConfirmPasswordVisibility();
    void onLanguageChanged();  // Новый слот для смены языка

private:
    void setupUI();
    void setupLoginUI();
    void setupRegisterUI();
    void switchToLogin();
    void switchToRegister();
    bool validateEmail(const QString &email);
    bool validatePassword(const QString &password);
    void updatePasswordStrength(const QString &password);

    // UI elements
    QLabel *titleLabel;
    QLabel *subtitleLabel;

    // Common fields
    QLabel *emailLabel;
    QLineEdit *emailEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QPushButton *togglePasswordBtn;

    // Registration specific fields
    QLabel *confirmPasswordLabel;
    QLineEdit *confirmPasswordEdit;
    QPushButton *toggleConfirmPasswordBtn;
    QLabel *passwordStrengthLabel;
    QProgressBar *passwordStrengthBar;
    QLabel *passwordRequirementsLabel;

    // Validation indicators
    QLabel *emailValidIcon;
    QLabel *passwordValidIcon;
    QLabel *confirmPasswordValidIcon;

    // Buttons
    QPushButton *primaryBtn;
    QPushButton *switchModeBtn;

    bool isLoginMode;
    bool emailValid;
    bool passwordValid;
    bool confirmPasswordValid;
};

#endif // AUTHDIALOG_H
