#include "addentrydialog.h"
#include "mainwindow.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>

AddEntryDialog::AddEntryDialog(QWidget *parent)
    : QDialog(parent),
    dateLabel(nullptr),
    systolicLabel(nullptr),
    diastolicLabel(nullptr),
    pulseLabel(nullptr),
    dateEdit(nullptr),
    systolicEdit(nullptr),
    diastolicEdit(nullptr),
    pulseEdit(nullptr),
    saveBtn(nullptr),
    cancelBtn(nullptr)
{
    setupUI();
    data.date = QDate::currentDate();
    dateEdit->setText(data.date.toString("yyyy-MM-dd"));

    // Подключаем сигнал смены языка
    connect(&TranslationManager::instance(), &TranslationManager::languageChanged,
            this, &AddEntryDialog::onLanguageChanged);

    retranslateUi();
}

void AddEntryDialog::setupUI()
{
    setWindowTitle(TR("Add Measurement"));
    setFixedSize(400, 200);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    dateLabel = new QLabel();
    dateEdit = new QLineEdit();
    dateEdit->setPlaceholderText("YYYY-MM-DD");

    systolicLabel = new QLabel();
    systolicEdit = new QLineEdit();

    diastolicLabel = new QLabel();
    diastolicEdit = new QLineEdit();

    pulseLabel = new QLabel();
    pulseEdit = new QLineEdit();

    formLayout->addRow(dateLabel, dateEdit);
    formLayout->addRow(systolicLabel, systolicEdit);
    formLayout->addRow(diastolicLabel, diastolicEdit);
    formLayout->addRow(pulseLabel, pulseEdit);

    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    saveBtn = new QPushButton();
    saveBtn->setStyleSheet("QPushButton {"
                           "background-color: #4CAF50;"
                           "color: white;"
                           "border: none;"
                           "padding: 8px 16px;"
                           "border-radius: 4px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #45a049;"
                           "}");

    cancelBtn = new QPushButton();
    cancelBtn->setStyleSheet("QPushButton {"
                             "background-color: #757575;"
                             "color: white;"
                             "border: none;"
                             "padding: 8px 16px;"
                             "border-radius: 4px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #5d5d5d;"
                             "}");

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(saveBtn, &QPushButton::clicked, this, &AddEntryDialog::saveEntry);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddEntryDialog::saveEntry()
{
    // Проверка данных
    data.date = QDate::fromString(dateEdit->text(), "yyyy-MM-dd");
    if (!data.date.isValid()) {
        QMessageBox::warning(this, TR("Error"), TR("Invalid date format. Use YYYY-MM-DD."));
        return;
    }

    bool ok;
    data.systolic = systolicEdit->text().toInt(&ok);
    if (!ok || data.systolic <= 0 || data.systolic > 300) {
        QMessageBox::warning(this, TR("Error"), TR("Invalid systolic value."));
        return;
    }

    data.diastolic = diastolicEdit->text().toInt(&ok);
    if (!ok || data.diastolic <= 0 || data.diastolic > 200) {
        QMessageBox::warning(this, TR("Error"), TR("Invalid diastolic value."));
        return;
    }

    data.pulse = pulseEdit->text().toInt(&ok);
    if (!ok || data.pulse <= 0 || data.pulse > 200) {
        QMessageBox::warning(this, TR("Error"), TR("Invalid pulse value."));
        return;
    }

    accept();
}

void AddEntryDialog::onLanguageChanged()
{
    retranslateUi();
}

void AddEntryDialog::retranslateUi()
{
    setWindowTitle(TR("Add Measurement"));
    dateLabel->setText(TR("Date (YYYY-MM-DD)"));
    systolicLabel->setText(TR("Systolic Pressure"));
    diastolicLabel->setText(TR("Diastolic Pressure"));
    pulseLabel->setText(TR("Pulse"));
    saveBtn->setText(TR("Save"));
    cancelBtn->setText(TR("Cancel"));
}

void AddEntryDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QDialog::changeEvent(event);
}
