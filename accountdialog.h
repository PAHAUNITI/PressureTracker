#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include "translations.h"

class QLabel;
class QLineEdit;
class QDateEdit;
class QComboBox;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;
class QGroupBox;

class AccountDialog : public QDialog
{
    Q_OBJECT

public:
    AccountDialog(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override; // Добавлено

private slots:
    void saveSettings();
    void logout();
    void enableSaveButton();
    void retranslateUi();
    void onLanguageChanged();
    void changePassword();
    void validatePasswordFields();
    void changePhoto();
    void loadAvatarFromFile();

private:
    void setupUI();
    void loadSettings();
    void applyModernStyle();
    void saveAvatarToSettings(const QPixmap& pixmap);
    QPixmap loadAvatarFromSettings();
    void updateAvatarDisplay();

    // UI elements
    QLabel *profileTitle;
    QLabel *avatarLabel;
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QLineEdit *emailEdit;
    QDateEdit *birthDateEdit;
    QComboBox *languageCombo;
    QPushButton *saveBtn;
    QPushButton *logoutBtn;
    QPushButton *cancelBtn;
    QPushButton *changePhotoBtn;

    // Password change elements
    QLineEdit *currentPasswordEdit;
    QLineEdit *newPasswordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *changePasswordBtn;
    QGroupBox *passwordGroup;

    // Avatar
    QPixmap currentAvatar;
};

#endif // ACCOUNTDIALOG_H
