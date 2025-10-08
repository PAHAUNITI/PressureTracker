#ifndef ADDENTRYDIALOG_H
#define ADDENTRYDIALOG_H

#include <QDialog>
#include <QEvent>
#include "datatypes.h"
#include "translations.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;

class AddEntryDialog : public QDialog
{
    Q_OBJECT

public:
    AddEntryDialog(QWidget *parent = nullptr);
    PressureData getData() const { return data; }

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void saveEntry();
    void retranslateUi();
    void onLanguageChanged();

private:
    void setupUI();

    PressureData data;

    // UI elements
    QLabel *dateLabel;
    QLabel *systolicLabel;
    QLabel *diastolicLabel;
    QLabel *pulseLabel;

    QLineEdit *dateEdit;
    QLineEdit *systolicEdit;
    QLineEdit *diastolicEdit;
    QLineEdit *pulseEdit;

    QPushButton *saveBtn;
    QPushButton *cancelBtn;
};

#endif // ADDENTRYDIALOG_H
