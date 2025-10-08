#ifndef SUPPORTDIALOG_H
#define SUPPORTDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QPixmap>
#include "translations.h"

// Forward declarations
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class SupportDialog : public QDialog
{
    Q_OBJECT

public:
    SupportDialog(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void retranslateUi();
    void onLanguageChanged();
    void openWebsite();

private:
    void setupUI();
    void updateLinkDisplay();

    // UI elements
    QLabel *supportLabel;
    QLabel *thankYouLabel;
    QLabel *linkLabel;
    QPushButton *closeBtn;
    QPushButton *openWebsiteBtn;
};

#endif // SUPPORTDIALOG_H
