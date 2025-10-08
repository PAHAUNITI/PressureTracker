#include "accountdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QDateEdit>
#include <QLineEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpacerItem>
#include <QFont>
#include <QDate>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QBuffer>
#include <QStandardPaths>
#include <QPainter>
#include <QPainterPath>

AccountDialog::AccountDialog(QWidget *parent)
    : QDialog(parent),
    profileTitle(nullptr),
    avatarLabel(nullptr),
    firstNameEdit(nullptr),
    lastNameEdit(nullptr),
    emailEdit(nullptr),
    birthDateEdit(nullptr),
    languageCombo(nullptr),
    saveBtn(nullptr),
    logoutBtn(nullptr),
    cancelBtn(nullptr),
    changePhotoBtn(nullptr),
    currentPasswordEdit(nullptr),
    newPasswordEdit(nullptr),
    confirmPasswordEdit(nullptr),
    changePasswordBtn(nullptr),
    passwordGroup(nullptr)
{
    setupUI();
    loadSettings();
    applyModernStyle();

    // Подключаем сигнал смены языка
    connect(&TranslationManager::instance(), &TranslationManager::languageChanged,
            this, &AccountDialog::onLanguageChanged);

    retranslateUi();
}

void AccountDialog::setupUI()
{
    setWindowTitle(TR("My Account"));
    setFixedSize(485, 655);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(13);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // Profile Header
    QHBoxLayout *profileLayout = new QHBoxLayout();

    // Avatar placeholder
    avatarLabel = new QLabel();
    avatarLabel->setFixedSize(62, 62);
    avatarLabel->setStyleSheet("QLabel {"
                               "background-color: #e0e0e0;"
                               "border-radius: 31px;"
                               "border: 2px solid #4361ee;"
                               "}");
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setCursor(Qt::PointingHandCursor);
    avatarLabel->setToolTip(TR("Click to change photo"));

    // Make avatar label clickable
    avatarLabel->installEventFilter(this);

    QVBoxLayout *profileTextLayout = new QVBoxLayout();
    profileTitle = new QLabel();
    profileTitle->setStyleSheet("QLabel {"
                                "font-size: 18px;"
                                "font-weight: bold;"
                                "color: #2b2d42;"
                                "}");

    changePhotoBtn = new QPushButton(TR("Change Photo"));
    changePhotoBtn->setStyleSheet("QPushButton {"
                                  "color: #4361ee;"
                                  "border: none;"
                                  "background: transparent;"
                                  "padding: 3px;"
                                  "text-align: left;"
                                  "font-size: 6.5pt;"
                                  "}"
                                  "QPushButton:hover {"
                                  "color: #3a56d4;"
                                  "text-decoration: underline;"
                                  "}");

    profileTextLayout->addWidget(profileTitle);
    profileTextLayout->addWidget(changePhotoBtn);
    profileTextLayout->addStretch();

    profileLayout->addWidget(avatarLabel);
    profileLayout->addSpacing(10);
    profileLayout->addLayout(profileTextLayout);
    profileLayout->addStretch();

    // Personal Information Group
    QGroupBox *personalGroup = new QGroupBox(TR("Personal Information"));
    personalGroup->setStyleSheet("QGroupBox {"
                                 "font-weight: bold;"
                                 "font-size: 12px;"
                                 "color: #2b2d42;"
                                 "border: 1px solid #e9ecef;"
                                 "border-radius: 5px;"
                                 "margin-top: 8px;"
                                 "padding-top: 8px;"
                                 "}"
                                 "QGroupBox::title {"
                                 "subcontrol-origin: margin;"
                                 "left: 6px;"
                                 "padding: 0 5px 0 5px;"
                                 "}");

    QFormLayout *formLayout = new QFormLayout(personalGroup);
    formLayout->setSpacing(6);
    formLayout->setContentsMargins(10, 12, 10, 10);

    firstNameEdit = new QLineEdit();
    lastNameEdit = new QLineEdit();
    emailEdit = new QLineEdit();
    birthDateEdit = new QDateEdit();
    birthDateEdit->setCalendarPopup(true);
    birthDateEdit->setDisplayFormat("dd.MM.yyyy");

    // Set common styles for input fields with 6.5pt font
    QString inputStyle = "QLineEdit, QDateEdit {"
                         "border: 1px solid #e0e0e0;"
                         "border-radius: 4px;"
                         "padding: 5px 8px;"
                         "font-size: 6.5pt;"
                         "background-color: white;"
                         "min-height: 26px;"
                         "}"
                         "QLineEdit:focus, QDateEdit:focus {"
                         "border-color: #4361ee;"
                         "}";

    firstNameEdit->setStyleSheet(inputStyle);
    lastNameEdit->setStyleSheet(inputStyle);
    emailEdit->setStyleSheet(inputStyle);
    birthDateEdit->setStyleSheet(inputStyle);

    // Set font for input fields
    QFont inputFont;
    inputFont.setPointSizeF(6.5);
    firstNameEdit->setFont(inputFont);
    lastNameEdit->setFont(inputFont);
    emailEdit->setFont(inputFont);
    birthDateEdit->setFont(inputFont);

    // Set minimum width for better text display
    firstNameEdit->setMinimumWidth(185);
    lastNameEdit->setMinimumWidth(185);
    emailEdit->setMinimumWidth(185);
    birthDateEdit->setMinimumWidth(185);

    // Set label font
    QFont labelFont;
    labelFont.setPointSizeF(6.5);

    QLabel *firstNameLabel = new QLabel(TR("First Name:"));
    QLabel *lastNameLabel = new QLabel(TR("Last Name:"));
    QLabel *emailLabel = new QLabel(TR("Email:"));
    QLabel *birthDateLabel = new QLabel(TR("Date of Birth:"));

    firstNameLabel->setFont(labelFont);
    lastNameLabel->setFont(labelFont);
    emailLabel->setFont(labelFont);
    birthDateLabel->setFont(labelFont);

    formLayout->addRow(firstNameLabel, firstNameEdit);
    formLayout->addRow(lastNameLabel, lastNameEdit);
    formLayout->addRow(emailLabel, emailEdit);
    formLayout->addRow(birthDateLabel, birthDateEdit);

    // Password Change Group
    passwordGroup = new QGroupBox(TR("Change Password"));
    passwordGroup->setStyleSheet(personalGroup->styleSheet());

    QVBoxLayout *passwordMainLayout = new QVBoxLayout(passwordGroup);
    passwordMainLayout->setContentsMargins(10, 12, 10, 10);

    // Add top spacer to push content down by 6 pixels
    passwordMainLayout->addSpacing(6);

    QFormLayout *passwordLayout = new QFormLayout();
    passwordLayout->setSpacing(6);
    passwordLayout->setContentsMargins(0, 0, 0, 0);

    currentPasswordEdit = new QLineEdit();
    currentPasswordEdit->setEchoMode(QLineEdit::Password);
    currentPasswordEdit->setMinimumHeight(26);

    newPasswordEdit = new QLineEdit();
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setMinimumHeight(26);

    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setMinimumHeight(26);

    // Apply styles to password fields with 6.5pt font
    QString passwordInputStyle = "QLineEdit {"
                                 "border: 1px solid #e0e0e0;"
                                 "border-radius: 4px;"
                                 "padding: 5px 8px;"
                                 "font-size: 6.5pt;"
                                 "background-color: white;"
                                 "min-height: 26px;"
                                 "}"
                                 "QLineEdit:focus {"
                                 "border-color: #4361ee;"
                                 "}";

    currentPasswordEdit->setStyleSheet(passwordInputStyle);
    newPasswordEdit->setStyleSheet(passwordInputStyle);
    confirmPasswordEdit->setStyleSheet(passwordInputStyle);

    // Set font for password fields
    currentPasswordEdit->setFont(inputFont);
    newPasswordEdit->setFont(inputFont);
    confirmPasswordEdit->setFont(inputFont);

    // Set minimum width for better text display
    currentPasswordEdit->setMinimumWidth(185);
    newPasswordEdit->setMinimumWidth(185);
    confirmPasswordEdit->setMinimumWidth(185);

    // Create labels for password fields
    QLabel *currentPasswordLabel = new QLabel(TR("Current Password:"));
    QLabel *newPasswordLabel = new QLabel(TR("New Password:"));
    QLabel *confirmPasswordLabel = new QLabel(TR("Confirm Password:"));

    currentPasswordLabel->setFont(labelFont);
    newPasswordLabel->setFont(labelFont);
    confirmPasswordLabel->setFont(labelFont);

    passwordLayout->addRow(currentPasswordLabel, currentPasswordEdit);
    passwordLayout->addRow(newPasswordLabel, newPasswordEdit);
    passwordLayout->addRow(confirmPasswordLabel, confirmPasswordEdit);

    changePasswordBtn = new QPushButton(TR("Change Password"));
    changePasswordBtn->setFixedSize(125, 29);
    changePasswordBtn->setEnabled(false);

    QHBoxLayout *passwordButtonLayout = new QHBoxLayout();
    passwordButtonLayout->addStretch();
    passwordButtonLayout->addWidget(changePasswordBtn);

    passwordMainLayout->addLayout(passwordLayout);
    passwordMainLayout->addSpacing(6);
    passwordMainLayout->addLayout(passwordButtonLayout);

    // Preferences Group
    QGroupBox *preferencesGroup = new QGroupBox(TR("Preferences"));
    preferencesGroup->setStyleSheet(personalGroup->styleSheet());

    QFormLayout *prefLayout = new QFormLayout(preferencesGroup);
    prefLayout->setSpacing(6);
    prefLayout->setContentsMargins(10, 12, 10, 10);

    languageCombo = new QComboBox();
    languageCombo->addItem(TR("English"), "en");
    languageCombo->addItem(TR("Russian"), "ru");

    languageCombo->setStyleSheet("QComboBox {"
                                 "border: 1px solid #e0e0e0;"
                                 "border-radius: 4px;"
                                 "padding: 5px 8px;"
                                 "font-size: 6.5pt;"
                                 "background-color: white;"
                                 "min-height: 26px;"
                                 "}"
                                 "QComboBox:focus {"
                                 "border-color: #4361ee;"
                                 "}"
                                 "QComboBox::drop-down {"
                                 "border: none;"
                                 "}"
                                 "QComboBox::down-arrow {"
                                 "image: none;"
                                 "border-left: 1px solid #e0e0e0;"
                                 "padding: 0 5px;"
                                 "}");

    languageCombo->setFont(inputFont);
    languageCombo->setMinimumWidth(185);

    QLabel *languageLabel = new QLabel(TR("Language:"));
    languageLabel->setFont(labelFont);
    prefLayout->addRow(languageLabel, languageCombo);

    // Buttons Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    logoutBtn = new QPushButton();
    logoutBtn->setFixedSize(85, 29);

    saveBtn = new QPushButton();
    saveBtn->setFixedSize(85, 29);

    cancelBtn = new QPushButton();
    cancelBtn->setFixedSize(85, 29);

    buttonLayout->addWidget(logoutBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(saveBtn);

    // Add all to main layout
    mainLayout->addLayout(profileLayout);
    mainLayout->addWidget(personalGroup);
    mainLayout->addWidget(passwordGroup);
    mainLayout->addWidget(preferencesGroup);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(saveBtn, &QPushButton::clicked, this, &AccountDialog::saveSettings);
    connect(logoutBtn, &QPushButton::clicked, this, &AccountDialog::logout);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(changePhotoBtn, &QPushButton::clicked, this, &AccountDialog::changePhoto);
    connect(changePasswordBtn, &QPushButton::clicked, this, &AccountDialog::changePassword);

    // Connect text changes to enable save button
    connect(firstNameEdit, &QLineEdit::textChanged, this, &AccountDialog::enableSaveButton);
    connect(lastNameEdit, &QLineEdit::textChanged, this, &AccountDialog::enableSaveButton);
    connect(emailEdit, &QLineEdit::textChanged, this, &AccountDialog::enableSaveButton);
    connect(birthDateEdit, &QDateEdit::dateChanged, this, &AccountDialog::enableSaveButton);
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AccountDialog::enableSaveButton);

    // Connect password field changes
    connect(currentPasswordEdit, &QLineEdit::textChanged, this, &AccountDialog::validatePasswordFields);
    connect(newPasswordEdit, &QLineEdit::textChanged, this, &AccountDialog::validatePasswordFields);
    connect(confirmPasswordEdit, &QLineEdit::textChanged, this, &AccountDialog::validatePasswordFields);
}

void AccountDialog::loadSettings()
{
    QSettings settings;

    // Load personal information
    firstNameEdit->setText(settings.value("user/firstName", "John").toString());
    lastNameEdit->setText(settings.value("user/lastName", "Doe").toString());
    emailEdit->setText(settings.value("user/email", "john.doe@example.com").toString());

    QDate birthDate = settings.value("user/birthDate", QDate(1990, 1, 1)).toDate();
    birthDateEdit->setDate(birthDate);

    // Load language
    QString language = settings.value("language", "en").toString();
    int index = languageCombo->findData(language);
    if (index >= 0) {
        languageCombo->setCurrentIndex(index);
    }

    // Load avatar
    currentAvatar = loadAvatarFromSettings();
    updateAvatarDisplay();

    saveBtn->setEnabled(false);
}

void AccountDialog::applyModernStyle()
{
    // Set button font
    QFont buttonFont;
    buttonFont.setPointSizeF(6.5);
    buttonFont.setBold(true);

    saveBtn->setFont(buttonFont);
    logoutBtn->setFont(buttonFont);
    cancelBtn->setFont(buttonFont);
    changePasswordBtn->setFont(buttonFont);

    // Apply modern button styles
    saveBtn->setStyleSheet("QPushButton {"
                           "background-color: #4361ee;"
                           "color: white;"
                           "border: none;"
                           "border-radius: 4px;"
                           "min-height: 29px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #3a56d4;"
                           "}"
                           "QPushButton:disabled {"
                           "background-color: #cccccc;"
                           "color: #666666;"
                           "}");

    logoutBtn->setStyleSheet("QPushButton {"
                             "background-color: #f44336;"
                             "color: white;"
                             "border: none;"
                             "border-radius: 4px;"
                             "min-height: 29px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #da190b;"
                             "}");

    cancelBtn->setStyleSheet("QPushButton {"
                             "background-color: #6c757d;"
                             "color: white;"
                             "border: none;"
                             "border-radius: 4px;"
                             "min-height: 29px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #5a6268;"
                             "}");

    changePasswordBtn->setStyleSheet("QPushButton {"
                                     "background-color: #7209b7;"
                                     "color: white;"
                                     "border: none;"
                                     "border-radius: 4px;"
                                     "min-height: 29px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "background-color: #5a08a0;"
                                     "}"
                                     "QPushButton:disabled {"
                                     "background-color: #cccccc;"
                                     "color: #666666;"
                                     "}");
}

void AccountDialog::saveSettings()
{
    QSettings settings;

    // Save personal information
    settings.setValue("user/firstName", firstNameEdit->text());
    settings.setValue("user/lastName", lastNameEdit->text());
    settings.setValue("user/email", emailEdit->text());
    settings.setValue("user/birthDate", birthDateEdit->date());

    // Save language
    QString language = languageCombo->currentData().toString();
    settings.setValue("language", language);

    // Save avatar if changed
    if (!currentAvatar.isNull()) {
        saveAvatarToSettings(currentAvatar);
    }

    QMessageBox::information(this, TR("Success"),
                             TR("Settings saved successfully.\nSome changes may require restart to take effect."));

    saveBtn->setEnabled(false);
    accept();
}

void AccountDialog::logout()
{
    if (QMessageBox::question(this, TR("Log Out"),
                              TR("Are you sure you want to log out?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {

        QSettings settings;
        settings.setValue("isLoggedIn", false);

        QMessageBox::information(this, TR("Logged Out"), TR("You have been successfully logged out."));
        accept();
        qApp->quit();
    }
}

void AccountDialog::changePassword()
{
    QString currentPassword = currentPasswordEdit->text();
    QString newPassword = newPasswordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();

    // Validate current password (in real app, you'd verify against stored hash)
    QSettings settings;
    QString storedPasswordHash = settings.value("user/passwordHash", "").toString();

    // For demo purposes, if no password is set, consider any current password as valid
    // In production, you should always require current password verification
    if (!storedPasswordHash.isEmpty()) {
        QString currentPasswordHash = QString(QCryptographicHash::hash(
                                                  currentPassword.toUtf8(), QCryptographicHash::Sha256).toHex());

        if (currentPasswordHash != storedPasswordHash) {
            QMessageBox::warning(this, TR("Error"), TR("Current password is incorrect."));
            return;
        }
    }

    // Validate new password
    if (newPassword.length() < 6) {
        QMessageBox::warning(this, TR("Error"), TR("New password must be at least 6 characters long."));
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, TR("Error"), TR("New passwords do not match."));
        return;
    }

    // Save new password hash
    QString newPasswordHash = QString(QCryptographicHash::hash(
                                          newPassword.toUtf8(), QCryptographicHash::Sha256).toHex());
    settings.setValue("user/passwordHash", newPasswordHash);

    // Clear password fields
    currentPasswordEdit->clear();
    newPasswordEdit->clear();
    confirmPasswordEdit->clear();

    QMessageBox::information(this, TR("Success"), TR("Password changed successfully."));
    changePasswordBtn->setEnabled(false);
}

void AccountDialog::validatePasswordFields()
{
    bool hasCurrentPassword = !currentPasswordEdit->text().isEmpty();
    bool hasNewPassword = !newPasswordEdit->text().isEmpty();
    bool hasConfirmPassword = !confirmPasswordEdit->text().isEmpty();

    changePasswordBtn->setEnabled(hasCurrentPassword && hasNewPassword && hasConfirmPassword);
}

void AccountDialog::enableSaveButton()
{
    saveBtn->setEnabled(true);
}

void AccountDialog::changePhoto()
{
    QStringList imageFilters;
    imageFilters << "Image files (*.png *.jpg *.jpeg *.bmp *.gif)"
                 << "PNG files (*.png)"
                 << "JPEG files (*.jpg *.jpeg)"
                 << "BMP files (*.bmp)"
                 << "GIF files (*.gif)";

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    TR("Select Profile Picture"),
                                                    QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                                                    imageFilters.join(";;"));

    if (!fileName.isEmpty()) {
        QPixmap newAvatar(fileName);
        if (!newAvatar.isNull()) {
            // Resize to fit our avatar display
            QPixmap scaledAvatar = newAvatar.scaled(62, 62, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Create circular mask
            QPixmap circularAvatar(62, 62);
            circularAvatar.fill(Qt::transparent);

            QPainter painter(&circularAvatar);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);

            QPainterPath path;
            path.addEllipse(0, 0, 62, 62);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, scaledAvatar);

            currentAvatar = circularAvatar;
            updateAvatarDisplay();
            enableSaveButton();

            QMessageBox::information(this, TR("Success"), TR("Profile picture updated successfully."));
        } else {
            QMessageBox::warning(this, TR("Error"), TR("Failed to load the selected image."));
        }
    }
}

void AccountDialog::loadAvatarFromFile()
{
    changePhoto();
}

void AccountDialog::updateAvatarDisplay()
{
    if (!currentAvatar.isNull()) {
        avatarLabel->setPixmap(currentAvatar);
        avatarLabel->setStyleSheet("QLabel {"
                                   "border-radius: 31px;"
                                   "border: 2px solid #4361ee;"
                                   "}");
    } else {
        // Set default avatar with initial letter
        QString firstName = firstNameEdit->text();
        QString lastName = lastNameEdit->text();
        QString initial = "";

        if (!firstName.isEmpty()) {
            initial = firstName.left(1).toUpper();
        } else if (!lastName.isEmpty()) {
            initial = lastName.left(1).toUpper();
        } else {
            initial = "U";
        }

        // Create default avatar with initial
        QPixmap defaultAvatar(62, 62);
        defaultAvatar.fill(QColor("#4361ee"));

        QPainter painter(&defaultAvatar);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(defaultAvatar.rect(), Qt::AlignCenter, initial);

        avatarLabel->setPixmap(defaultAvatar);
        avatarLabel->setStyleSheet("QLabel {"
                                   "border-radius: 31px;"
                                   "border: 2px solid #4361ee;"
                                   "}");
    }
}

void AccountDialog::saveAvatarToSettings(const QPixmap& pixmap)
{
    if (!pixmap.isNull()) {
        QByteArray avatarData;
        QBuffer buffer(&avatarData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "PNG");

        QSettings settings;
        settings.setValue("user/avatar", avatarData.toBase64());
    }
}

QPixmap AccountDialog::loadAvatarFromSettings()
{
    QSettings settings;
    QString avatarData = settings.value("user/avatar").toString();

    if (!avatarData.isEmpty()) {
        QByteArray imageData = QByteArray::fromBase64(avatarData.toUtf8());
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            return pixmap;
        }
    }

    return QPixmap();
}

void AccountDialog::onLanguageChanged()
{
    retranslateUi();
}

void AccountDialog::retranslateUi()
{
    setWindowTitle(TR("My Account"));
    profileTitle->setText(TR("My Account"));

    // Update group titles
    QList<QGroupBox*> groups = findChildren<QGroupBox*>();
    for (QGroupBox *group : groups) {
        if (group->title() == "Personal Information") {
            group->setTitle(TR("Personal Information"));
        } else if (group->title() == "Preferences") {
            group->setTitle(TR("Preferences"));
        } else if (group->title() == "Change Password") {
            group->setTitle(TR("Change Password"));
        }
    }

    // Update buttons
    saveBtn->setText(TR("Save Settings"));
    logoutBtn->setText(TR("Log Out"));
    cancelBtn->setText(TR("Cancel"));
    changePhotoBtn->setText(TR("Change Photo"));
    changePasswordBtn->setText(TR("Change Password"));

    // Update form labels
    QList<QFormLayout*> formLayouts = findChildren<QFormLayout*>();
    for (QFormLayout *formLayout : formLayouts) {
        for (int i = 0; i < formLayout->rowCount(); ++i) {
            QLayoutItem *labelItem = formLayout->itemAt(i, QFormLayout::LabelRole);
            if (labelItem && labelItem->widget()) {
                QLabel *label = qobject_cast<QLabel*>(labelItem->widget());
                if (label) {
                    QString labelText = label->text();
                    if (labelText.startsWith("First Name:")) label->setText(TR("First Name:"));
                    else if (labelText.startsWith("Last Name:")) label->setText(TR("Last Name:"));
                    else if (labelText.startsWith("Email:")) label->setText(TR("Email:"));
                    else if (labelText.startsWith("Date of Birth:")) label->setText(TR("Date of Birth:"));
                    else if (labelText.startsWith("Language:")) label->setText(TR("Language:"));
                    else if (labelText.startsWith("Current Password:")) label->setText(TR("Current Password:"));
                    else if (labelText.startsWith("New Password:")) label->setText(TR("New Password:"));
                    else if (labelText.startsWith("Confirm Password:")) label->setText(TR("Confirm Password:"));
                }
            }
        }
    }
}

void AccountDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QDialog::changeEvent(event);
}

// Add event filter for avatar label click
bool AccountDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == avatarLabel && event->type() == QEvent::MouseButtonPress) {
        changePhoto();
        return true;
    }
    return QDialog::eventFilter(obj, event);
}
