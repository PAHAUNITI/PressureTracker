#include "authdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include <QFrame>
#include <QProgressBar>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QCheckBox>

AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent), isLoginMode(true),
    emailValid(false), passwordValid(false), confirmPasswordValid(false)
{
    setupUI();

    // Подключаем сигнал смены языка
    connect(&TranslationManager::instance(), &TranslationManager::languageChanged,
            this, &AuthDialog::onLanguageChanged);

    retranslateUi();
}

void AuthDialog::setupUI()
{
    setWindowTitle(TR("PressureTracker - Authentication"));
    setFixedSize(500, 600);
    setModal(true);

    // Основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Заголовок
    titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2b2d42; margin-bottom: 5px;");

    subtitleLabel = new QLabel();
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("color: #6c757d; font-size: 14px; margin-bottom: 30px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);

    setupLoginUI();

    mainLayout->addStretch();
}

void AuthDialog::setupLoginUI()
{
    // Email field
    QFrame *emailFrame = new QFrame();
    emailFrame->setFixedHeight(50);
    emailFrame->setStyleSheet("QFrame { background-color: white; border: 2px solid #e0e0e0; border-radius: 8px; padding: 5px; }");
    QHBoxLayout *emailLayout = new QHBoxLayout(emailFrame);
    emailLayout->setContentsMargins(10, 5, 10, 5);

    emailLabel = new QLabel();
    emailLabel->setFixedWidth(120);
    emailLabel->setStyleSheet("color: #495057; font-weight: bold;");

    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText(TR("your@email.com"));
    emailEdit->setStyleSheet("QLineEdit { border: none; background: transparent; font-size: 14px; }");

    emailValidIcon = new QLabel();
    emailValidIcon->setFixedSize(20, 20);
    emailValidIcon->setStyleSheet("QLabel { border: none; }");

    emailLayout->addWidget(emailLabel);
    emailLayout->addWidget(emailEdit);
    emailLayout->addWidget(emailValidIcon);

    // Password field
    QFrame *passwordFrame = new QFrame();
    passwordFrame->setFixedHeight(50);
    passwordFrame->setStyleSheet("QFrame { background-color: white; border: 2px solid #e0e0e0; border-radius: 8px; padding: 5px; }");
    QHBoxLayout *passwordLayout = new QHBoxLayout(passwordFrame);
    passwordLayout->setContentsMargins(10, 5, 10, 5);

    passwordLabel = new QLabel();
    passwordLabel->setFixedWidth(120);
    passwordLabel->setStyleSheet("color: #495057; font-weight: bold;");

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText(TR("Enter your password"));
    passwordEdit->setStyleSheet("QLineEdit { border: none; background: transparent; font-size: 14px; }");

    togglePasswordBtn = new QPushButton("👁");
    togglePasswordBtn->setFixedSize(30, 30);
    togglePasswordBtn->setStyleSheet("QPushButton { border: none; background: transparent; font-size: 16px; }"
                                     "QPushButton:hover { background-color: #f8f9fa; border-radius: 4px; }");

    passwordValidIcon = new QLabel();
    passwordValidIcon->setFixedSize(20, 20);
    passwordValidIcon->setStyleSheet("QLabel { border: none; }");

    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(togglePasswordBtn);
    passwordLayout->addWidget(passwordValidIcon);

    // Buttons
    primaryBtn = new QPushButton();
    primaryBtn->setFixedHeight(45);
    primaryBtn->setStyleSheet("QPushButton {"
                              "background-color: #4361ee;"
                              "color: white;"
                              "border: none;"
                              "border-radius: 8px;"
                              "font-weight: bold;"
                              "font-size: 16px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #3a56d4;"
                              "}"
                              "QPushButton:disabled {"
                              "background-color: #cccccc;"
                              "color: #666666;"
                              "}");

    switchModeBtn = new QPushButton();
    switchModeBtn->setStyleSheet("QPushButton {"
                                 "color: #4361ee;"
                                 "border: none;"
                                 "background: transparent;"
                                 "padding: 10px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "color: #3a56d4;"
                                 "text-decoration: underline;"
                                 "}");

    // Add to main layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        // Remove existing widgets except title and subtitle
        while (mainLayout->count() > 2) {
            QLayoutItem *item = mainLayout->takeAt(2);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }

        mainLayout->addWidget(emailFrame);
        mainLayout->addWidget(passwordFrame);
        mainLayout->addSpacing(10);
        mainLayout->addWidget(primaryBtn);
        mainLayout->addWidget(switchModeBtn);
    }

    // Connect signals
    connect(primaryBtn, &QPushButton::clicked, this, &AuthDialog::login);
    connect(switchModeBtn, &QPushButton::clicked, this, &AuthDialog::switchToRegister);
    connect(togglePasswordBtn, &QPushButton::clicked, this, &AuthDialog::togglePasswordVisibility);
    connect(emailEdit, &QLineEdit::textChanged, this, &AuthDialog::validateForm);
    connect(passwordEdit, &QLineEdit::textChanged, this, &AuthDialog::validateForm);
}

void AuthDialog::setupRegisterUI()
{
    // Email field
    QFrame *emailFrame = new QFrame();
    emailFrame->setFixedHeight(50);
    emailFrame->setStyleSheet("QFrame { background-color: white; border: 2px solid #e0e0e0; border-radius: 8px; padding: 5px; }");
    QHBoxLayout *emailLayout = new QHBoxLayout(emailFrame);
    emailLayout->setContentsMargins(10, 5, 10, 5);

    emailLabel = new QLabel();
    emailLabel->setFixedWidth(120);
    emailLabel->setStyleSheet("color: #495057; font-weight: bold;");

    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText(TR("your@email.com"));
    emailEdit->setStyleSheet("QLineEdit { border: none; background: transparent; font-size: 14px; }");

    emailValidIcon = new QLabel();
    emailValidIcon->setFixedSize(20, 20);

    emailLayout->addWidget(emailLabel);
    emailLayout->addWidget(emailEdit);
    emailLayout->addWidget(emailValidIcon);

    // Password field
    QFrame *passwordFrame = new QFrame();
    passwordFrame->setFixedHeight(50);
    passwordFrame->setStyleSheet("QFrame { background-color: white; border: 2px solid #e0e0e0; border-radius: 8px; padding: 5px; }");
    QHBoxLayout *passwordLayout = new QHBoxLayout(passwordFrame);
    passwordLayout->setContentsMargins(10, 5, 10, 5);

    passwordLabel = new QLabel();
    passwordLabel->setFixedWidth(120);
    passwordLabel->setStyleSheet("color: #495057; font-weight: bold;");

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText(TR("Create a strong password"));
    passwordEdit->setStyleSheet("QLineEdit { border: none; background: transparent; font-size: 14px; }");

    togglePasswordBtn = new QPushButton("👁");
    togglePasswordBtn->setFixedSize(30, 30);
    togglePasswordBtn->setStyleSheet("QPushButton { border: none; background: transparent; font-size: 16px; }"
                                     "QPushButton:hover { background-color: #f8f9fa; border-radius: 4px; }");

    passwordValidIcon = new QLabel();
    passwordValidIcon->setFixedSize(20, 20);

    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(togglePasswordBtn);
    passwordLayout->addWidget(passwordValidIcon);

    // Confirm password field
    QFrame *confirmPasswordFrame = new QFrame();
    confirmPasswordFrame->setFixedHeight(50);
    confirmPasswordFrame->setStyleSheet("QFrame { background-color: white; border: 2px solid #e0e0e0; border-radius: 8px; padding: 5px; }");
    QHBoxLayout *confirmPasswordLayout = new QHBoxLayout(confirmPasswordFrame);
    confirmPasswordLayout->setContentsMargins(10, 5, 10, 5);

    confirmPasswordLabel = new QLabel();
    confirmPasswordLabel->setFixedWidth(120);
    confirmPasswordLabel->setStyleSheet("color: #495057; font-weight: bold;");

    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setPlaceholderText(TR("Confirm your password"));
    confirmPasswordEdit->setStyleSheet("QLineEdit { border: none; background: transparent; font-size: 14px; }");

    toggleConfirmPasswordBtn = new QPushButton("👁");
    toggleConfirmPasswordBtn->setFixedSize(30, 30);
    toggleConfirmPasswordBtn->setStyleSheet("QPushButton { border: none; background: transparent; font-size: 16px; }"
                                            "QPushButton:hover { background-color: #f8f9fa; border-radius: 4px; }");

    confirmPasswordValidIcon = new QLabel();
    confirmPasswordValidIcon->setFixedSize(20, 20);

    confirmPasswordLayout->addWidget(confirmPasswordLabel);
    confirmPasswordLayout->addWidget(confirmPasswordEdit);
    confirmPasswordLayout->addWidget(toggleConfirmPasswordBtn);
    confirmPasswordLayout->addWidget(confirmPasswordValidIcon);

    // Password strength
    passwordStrengthBar = new QProgressBar();
    passwordStrengthBar->setFixedHeight(6);
    passwordStrengthBar->setTextVisible(false);
    passwordStrengthBar->setStyleSheet("QProgressBar {"
                                       "border: none;"
                                       "background-color: #e9ecef;"
                                       "border-radius: 3px;"
                                       "}"
                                       "QProgressBar::chunk {"
                                       "background-color: #dc3545;"
                                       "border-radius: 3px;"
                                       "}");

    passwordStrengthLabel = new QLabel();
    passwordStrengthLabel->setStyleSheet("color: #6c757d; font-size: 12px;");

    // Password requirements
    passwordRequirementsLabel = new QLabel();
    passwordRequirementsLabel->setFixedWidth(440);
    passwordRequirementsLabel->setStyleSheet("color: #6c757d; font-size: 12px; background-color: #f8f9fa; padding: 10px; border-radius: 6px;");
    passwordRequirementsLabel->setWordWrap(true);

    // Buttons
    primaryBtn = new QPushButton();
    primaryBtn->setFixedHeight(45);
    primaryBtn->setEnabled(false);
    primaryBtn->setStyleSheet("QPushButton {"
                              "background-color: #4361ee;"
                              "color: white;"
                              "border: none;"
                              "border-radius: 8px;"
                              "font-weight: bold;"
                              "font-size: 16px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #3a56d4;"
                              "}"
                              "QPushButton:disabled {"
                              "background-color: #cccccc;"
                              "color: #666666;"
                              "}");

    switchModeBtn = new QPushButton();
    switchModeBtn->setStyleSheet("QPushButton {"
                                 "color: #4361ee;"
                                 "border: none;"
                                 "background: transparent;"
                                 "padding: 10px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "color: #3a56d4;"
                                 "text-decoration: underline;"
                                 "}");

    // Add to main layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        // Remove existing widgets except title and subtitle
        while (mainLayout->count() > 2) {
            QLayoutItem *item = mainLayout->takeAt(2);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }

        mainLayout->addWidget(emailFrame);
        mainLayout->addWidget(passwordFrame);
        mainLayout->addWidget(confirmPasswordFrame);
        mainLayout->addWidget(passwordStrengthBar);
        mainLayout->addWidget(passwordStrengthLabel);
        mainLayout->addWidget(passwordRequirementsLabel);
        mainLayout->addSpacing(10);
        mainLayout->addWidget(primaryBtn);
        mainLayout->addWidget(switchModeBtn);
    }

    // Connect signals
    connect(primaryBtn, &QPushButton::clicked, this, &AuthDialog::registerUser);
    connect(switchModeBtn, &QPushButton::clicked, this, &AuthDialog::switchToLogin);
    connect(togglePasswordBtn, &QPushButton::clicked, this, &AuthDialog::togglePasswordVisibility);
    connect(toggleConfirmPasswordBtn, &QPushButton::clicked, this, &AuthDialog::toggleConfirmPasswordVisibility);
    connect(emailEdit, &QLineEdit::textChanged, this, &AuthDialog::validateForm);
    connect(passwordEdit, &QLineEdit::textChanged, this, &AuthDialog::checkPasswordStrength);
    connect(passwordEdit, &QLineEdit::textChanged, this, &AuthDialog::validateForm);
    connect(confirmPasswordEdit, &QLineEdit::textChanged, this, &AuthDialog::validateForm);
}

void AuthDialog::switchToLogin()
{
    isLoginMode = true;
    setupLoginUI();
    retranslateUi();
}

void AuthDialog::switchToRegister()
{
    isLoginMode = false;
    setupRegisterUI();
    retranslateUi();
}

void AuthDialog::login()
{
    QString email = emailEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (!validateEmail(email)) {
        QMessageBox::warning(this, TR("Invalid Email"), TR("Please enter a valid email address."));
        emailEdit->setFocus();
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, TR("Invalid Password"), TR("Please enter your password."));
        passwordEdit->setFocus();
        return;
    }

    QSettings settings;
    QString savedEmail = settings.value("user/email").toString();
    QString savedPassword = settings.value("user/password").toString();

    if (email == savedEmail && password == savedPassword) {
        settings.setValue("isLoggedIn", true);
        accept();
    } else {
        QMessageBox::warning(this, TR("Login Failed"),
                             TR("Invalid email or password. Please check your credentials and try again."));
    }
}

void AuthDialog::registerUser()
{
    QString email = emailEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();

    if (!validateEmail(email)) {
        QMessageBox::warning(this, TR("Invalid Email"), TR("Please enter a valid email address."));
        emailEdit->setFocus();
        return;
    }

    if (!validatePassword(password)) {
        QMessageBox::warning(this, TR("Weak Password"),
                             TR("Please choose a stronger password that meets the requirements."));
        passwordEdit->setFocus();
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, TR("Password Mismatch"), TR("Passwords do not match. Please confirm your password."));
        confirmPasswordEdit->setFocus();
        return;
    }

    QSettings settings;
    settings.setValue("user/email", email);
    settings.setValue("user/password", password);
    settings.setValue("user/firstName", "");
    settings.setValue("user/lastName", "");
    settings.setValue("isLoggedIn", true);

    QMessageBox::information(this, TR("Registration Successful"),
                             TR("Your account has been created successfully!\n\nWelcome to PressureTracker!"));
    accept();
}

void AuthDialog::validateForm()
{
    QString email = emailEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (isLoginMode) {
        emailValid = validateEmail(email);
        passwordValid = !password.isEmpty();

        // Update icons
        emailValidIcon->setText(emailValid ? "✅" : (email.isEmpty() ? "" : "❌"));
        passwordValidIcon->setText(passwordValid ? "✅" : "");

        primaryBtn->setEnabled(emailValid && passwordValid);
    } else {
        QString confirmPassword = confirmPasswordEdit->text();

        emailValid = validateEmail(email);
        passwordValid = validatePassword(password);
        confirmPasswordValid = !confirmPassword.isEmpty() && password == confirmPassword;

        // Update icons
        emailValidIcon->setText(emailValid ? "✅" : (email.isEmpty() ? "" : "❌"));
        passwordValidIcon->setText(passwordValid ? "✅" : (password.isEmpty() ? "" : "❌"));
        confirmPasswordValidIcon->setText(confirmPasswordValid ? "✅" : (confirmPassword.isEmpty() ? "" : "❌"));

        primaryBtn->setEnabled(emailValid && passwordValid && confirmPasswordValid);
    }
}

void AuthDialog::checkPasswordStrength(const QString &password)
{
    updatePasswordStrength(password);

    // Update requirements text
    QStringList requirements;
    requirements << TR("• At least 8 characters") + (password.length() >= 8 ? " ✅" : " ❌");
    requirements << TR("• At least one uppercase letter") + (password.contains(QRegularExpression("[A-Z]")) ? " ✅" : " ❌");
    requirements << TR("• At least one lowercase letter") + (password.contains(QRegularExpression("[a-z]")) ? " ✅" : " ❌");
    requirements << TR("• At least one number") + (password.contains(QRegularExpression("[0-9]")) ? " ✅" : " ❌");
    requirements << TR("• At least one special character") + (password.contains(QRegularExpression("[^A-Za-z0-9]")) ? " ✅" : " ❌");

    if (passwordRequirementsLabel) {
        passwordRequirementsLabel->setText(requirements.join("\n"));
    }
}

void AuthDialog::updatePasswordStrength(const QString &password)
{
    if (!passwordStrengthBar || !passwordStrengthLabel) return;

    int strength = 0;
    QString strengthText;
    QString color;

    if (password.length() >= 8) strength += 20;
    if (password.contains(QRegularExpression("[A-Z]"))) strength += 20;
    if (password.contains(QRegularExpression("[a-z]"))) strength += 20;
    if (password.contains(QRegularExpression("[0-9]"))) strength += 20;
    if (password.contains(QRegularExpression("[^A-Za-z0-9]"))) strength += 20;

    if (strength <= 20) {
        strengthText = TR("Very Weak");
        color = "#dc3545";
    } else if (strength <= 40) {
        strengthText = TR("Weak");
        color = "#fd7e14";
    } else if (strength <= 60) {
        strengthText = TR("Fair");
        color = "#ffc107";
    } else if (strength <= 80) {
        strengthText = TR("Good");
        color = "#20c997";
    } else {
        strengthText = TR("Strong");
        color = "#198754";
    }

    passwordStrengthBar->setValue(strength);
    passwordStrengthBar->setStyleSheet(QString(
                                           "QProgressBar {"
                                           "border: none;"
                                           "background-color: #e9ecef;"
                                           "border-radius: 3px;"
                                           "}"
                                           "QProgressBar::chunk {"
                                           "background-color: %1;"
                                           "border-radius: 3px;"
                                           "}").arg(color));

    passwordStrengthLabel->setText(QString("%1: %2%").arg(strengthText).arg(strength));
}

bool AuthDialog::validateEmail(const QString &email)
{
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return emailRegex.match(email).hasMatch();
}

bool AuthDialog::validatePassword(const QString &password)
{
    if (password.length() < 8) return false;
    if (!password.contains(QRegularExpression("[A-Z]"))) return false;
    if (!password.contains(QRegularExpression("[a-z]"))) return false;
    if (!password.contains(QRegularExpression("[0-9]"))) return false;
    if (!password.contains(QRegularExpression("[^A-Za-z0-9]"))) return false;
    return true;
}

void AuthDialog::togglePasswordVisibility()
{
    if (passwordEdit->echoMode() == QLineEdit::Password) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordBtn->setText("🔒");
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
        togglePasswordBtn->setText("👁");
    }
}

void AuthDialog::toggleConfirmPasswordVisibility()
{
    if (confirmPasswordEdit->echoMode() == QLineEdit::Password) {
        confirmPasswordEdit->setEchoMode(QLineEdit::Normal);
        toggleConfirmPasswordBtn->setText("🔒");
    } else {
        confirmPasswordEdit->setEchoMode(QLineEdit::Password);
        toggleConfirmPasswordBtn->setText("👁");
    }
}

// НОВЫЙ МЕТОД: Обработчик смены языка
void AuthDialog::onLanguageChanged()
{
    retranslateUi();
}

void AuthDialog::retranslateUi()
{
    if (isLoginMode) {
        setWindowTitle(TR("Login - PressureTracker"));
        titleLabel->setText(TR("Welcome Back"));
        subtitleLabel->setText(TR("Sign in to your account to continue"));

        emailLabel->setText(TR("Email"));
        passwordLabel->setText(TR("Password"));

        primaryBtn->setText(TR("Sign In"));
        switchModeBtn->setText(TR("Don't have an account? Sign up"));

        emailEdit->setPlaceholderText(TR("your@email.com"));
        passwordEdit->setPlaceholderText(TR("Enter your password"));
    } else {
        setWindowTitle(TR("Create Account - PressureTracker"));
        titleLabel->setText(TR("Create Account"));
        subtitleLabel->setText(TR("Join PressureTracker to start tracking your health"));

        emailLabel->setText(TR("Email"));
        passwordLabel->setText(TR("Password"));
        confirmPasswordLabel->setText(TR("Confirm Password"));

        primaryBtn->setText(TR("Create Account"));
        switchModeBtn->setText(TR("Already have an account? Sign in"));

        emailEdit->setPlaceholderText(TR("your@email.com"));
        passwordEdit->setPlaceholderText(TR("Create a strong password"));
        confirmPasswordEdit->setPlaceholderText(TR("Confirm your password"));

        // Initial requirements text
        QStringList requirements;
        requirements << TR("• At least 8 characters") + " ❌";
        requirements << TR("• At least one uppercase letter") + " ❌";
        requirements << TR("• At least one lowercase letter") + " ❌";
        requirements << TR("• At least one number") + " ❌";
        requirements << TR("• At least one special character") + " ❌";

        if (passwordRequirementsLabel) {
            passwordRequirementsLabel->setText(requirements.join("\n"));
        }
    }
}

void AuthDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QDialog::changeEvent(event);
}
