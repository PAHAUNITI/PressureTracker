#include "supportdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>

SupportDialog::SupportDialog(QWidget *parent)
    : QDialog(parent),
    supportLabel(nullptr),
    thankYouLabel(nullptr),
    linkLabel(nullptr),
    closeBtn(nullptr),
    openWebsiteBtn(nullptr)
{
    setupUI();

    // Подключаем сигнал смены языка
    connect(&TranslationManager::instance(), &TranslationManager::languageChanged,
            this, &SupportDialog::onLanguageChanged);

    retranslateUi();
}

void SupportDialog::setupUI()
{
    setWindowTitle(TR("Support Developer"));
    setFixedSize(450, 400); // Уменьшили высоту, так как нет QR-кода
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    supportLabel = new QLabel();
    supportLabel->setWordWrap(true);
    supportLabel->setAlignment(Qt::AlignCenter);
    supportLabel->setStyleSheet("QLabel { font-size: 9pt; }");

    thankYouLabel = new QLabel();
    thankYouLabel->setWordWrap(true);
    thankYouLabel->setAlignment(Qt::AlignCenter);
    thankYouLabel->setStyleSheet("QLabel { font-size: 9pt; color: #4361ee; }");

    // Link section
    QLabel *linkTitleLabel = new QLabel(TR("Support Website:"));
    linkTitleLabel->setAlignment(Qt::AlignCenter);
    linkTitleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 10pt; color: #2b2d42; }");

    linkLabel = new QLabel();
    linkLabel->setText("<a href=\"https://www.life-line.ru/\">https://www.life-line.ru/</a>");
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setAlignment(Qt::AlignCenter);
    linkLabel->setStyleSheet("QLabel {"
                             "background-color: #f8f9fa;"
                             "border: 2px solid #4361ee;"
                             "border-radius: 8px;"
                             "padding: 15px;"
                             "font-size: 10pt;"
                             "color: #4361ee;"
                             "}");
    linkLabel->setTextFormat(Qt::RichText);
    linkLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    // Website button
    openWebsiteBtn = new QPushButton();
    openWebsiteBtn->setFixedSize(150, 35);
    openWebsiteBtn->setStyleSheet("QPushButton {"
                                  "background-color: #4361ee;"
                                  "color: white;"
                                  "border: none;"
                                  "padding: 8px 16px;"
                                  "border-radius: 4px;"
                                  "font-size: 9pt;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #3a56d4;"
                                  "}");

    closeBtn = new QPushButton();
    closeBtn->setFixedSize(100, 35);
    closeBtn->setStyleSheet("QPushButton {"
                            "background-color: #757575;"
                            "color: white;"
                            "border: none;"
                            "padding: 8px 16px;"
                            "border-radius: 4px;"
                            "font-size: 9pt;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #5d5d5d;"
                            "}");

    // Add all to main layout
    mainLayout->addWidget(supportLabel);
    mainLayout->addWidget(thankYouLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(linkTitleLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(linkLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(openWebsiteBtn, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);

    // Connect signals
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(openWebsiteBtn, &QPushButton::clicked, this, &SupportDialog::openWebsite);
}

void SupportDialog::openWebsite()
{
    QDesktopServices::openUrl(QUrl("https://www.life-line.ru/"));
}

void SupportDialog::onLanguageChanged()
{
    retranslateUi();
}

void SupportDialog::retranslateUi()
{
    setWindowTitle(TR("Support Developer"));

    if (supportLabel) {
        supportLabel->setText(TR("If you find this application useful and would like to support its development, "
                                 "you can make a donation to help continue improving the software."));
    }

    if (thankYouLabel) {
        thankYouLabel->setText(TR("Thank you for your support! ❤️"));
    }

    if (openWebsiteBtn) {
        openWebsiteBtn->setText(TR("Open Website"));
    }

    if (closeBtn) {
        closeBtn->setText(TR("Close"));
    }

    // Обновляем отображение ссылки
    updateLinkDisplay();
}

void SupportDialog::updateLinkDisplay()
{
    if (linkLabel) {
        linkLabel->setText("<a href=\"https://www.life-line.ru/\">https://www.life-line.ru/</a>");
    }
}

void SupportDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QDialog::changeEvent(event);
}
