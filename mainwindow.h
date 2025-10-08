#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTableWidgetItem>
#include <QEvent>
#include "datatypes.h" // Общая структура данных
#include "translations.h" // Добавляем нашу систему переводов

// Включаем заголовки Charts
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

// Forward declarations
class QTableWidget;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QGroupBox;
class QTranslator;
class QSettings;

// Используем namespace для Charts
QT_CHARTS_USE_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void addEntry();
    void showAccount();
    void showSupport();
    void updateChart();
    void onTableItemChanged(QTableWidgetItem *item);
    void togglePressureChart(bool visible);
    void togglePulseChart(bool visible);
    void onLanguageChanged();  // Новый слот для смены языка
    void retranslateUi();

private:
    void setupUI();
    void setupChart();
    void setupTable();
    void loadData();
    void saveData();
    void updateCurrentReadings();

    // UI элементы
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QHBoxLayout *centerLayout;
    QVBoxLayout *rightLayout;

    // Верхняя панель
    QLabel *appTitle;
    QPushButton *accountBtn;
    QPushButton *addEntryBtn;
    QPushButton *supportBtn;

    // Центральная часть - текущие показания
    QGroupBox *currentReadingsGroup;
    QLabel *pressureLabel;
    QLabel *pressureValue;
    QLabel *pulseLabel;
    QLabel *pulseValue;

    // Правая панель - индикаторы
    QGroupBox *indicatorsGroup;
    QWidget *pressureIndicator;
    QWidget *pulseIndicator;
    QPushButton *pressureChartBtn;
    QPushButton *pulseChartBtn;

    // График
    QGroupBox *chartGroup;
    QChartView *chartView;
    QChart *chart;
    QLineSeries *pressureSeries;
    QLineSeries *pulseSeries;
    QValueAxis *axisX;
    QValueAxis *axisYPressure;
    QValueAxis *axisYPulse;

    // Таблица
    QGroupBox *tableGroup;
    QTableWidget *dataTable;

    // Данные
    QList<PressureData> pressureData;
    QTranslator *translator;
};

#endif // MAINWINDOW_H
