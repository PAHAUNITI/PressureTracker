#include "mainwindow.h"
#include "authdialog.h"
#include "addentrydialog.h"
#include "accountdialog.h"
#include "supportdialog.h"
#include "datatypes.h"

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QGroupBox>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QDate>
#include <QMessageBox>
#include <QToolTip>
#include <algorithm>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), translator(nullptr)
{
    setupUI();
    loadData();
    updateCurrentReadings();
    updateChart();

    // Загрузка языка через нашу систему переводов
    QSettings settings;
    QString language = settings.value("language", "en").toString();
    TranslationManager::instance().setLanguage(language);
}

MainWindow::~MainWindow()
{
    saveData();
}

void MainWindow::setupUI()
{
    // Основные настройки окна
    setWindowTitle(TR("PressureTracker"));
    setMinimumSize(1000, 700);

    // Цветовая палитра
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::Base, Qt::white);
    setPalette(palette);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Верхняя панель
    topLayout = new QHBoxLayout();

    appTitle = new QLabel(TR("PressureTracker"));
    QFont titleFont = appTitle->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    appTitle->setFont(titleFont);

    // Кнопки верхней панели
    addEntryBtn = new QPushButton("+");
    addEntryBtn->setFixedSize(40, 40);
    addEntryBtn->setStyleSheet("QPushButton {"
                               "background-color: #4CAF50;"
                               "border-radius: 20px;"
                               "color: white;"
                               "font-weight: bold;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #45a049;"
                               "}");

    accountBtn = new QPushButton("👤");
    accountBtn->setFixedSize(40, 40);
    accountBtn->setStyleSheet("QPushButton {"
                              "background-color: #f44336;"
                              "border-radius: 20px;"
                              "color: white;"
                              "font-weight: bold;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #da190b;"
                              "}");

    supportBtn = new QPushButton("❤");
    supportBtn->setFixedSize(40, 40);
    supportBtn->setStyleSheet("QPushButton {"
                              "background-color: #757575;"
                              "border-radius: 20px;"
                              "color: white;"
                              "font-weight: bold;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #5d5d5d;"
                              "}");

    topLayout->addWidget(appTitle);
    topLayout->addStretch();
    topLayout->addWidget(addEntryBtn);
    topLayout->addWidget(accountBtn);
    topLayout->addWidget(supportBtn);

    mainLayout->addLayout(topLayout);

    // Основной контейнер для центральной части
    QWidget *centerContainer = new QWidget();
    QHBoxLayout *centerMainLayout = new QHBoxLayout(centerContainer);

    // Левая колонка - узкая (Current Readings + Measurement History)
    QVBoxLayout *leftColumnLayout = new QVBoxLayout();
    leftColumnLayout->setSpacing(15);

    // Current Readings
    currentReadingsGroup = new QGroupBox();
    currentReadingsGroup->setStyleSheet("QGroupBox {"
                                        "border: 2px solid #cccccc;"
                                        "border-radius: 10px;"
                                        "margin-top: 1ex;"
                                        "padding: 10px;"
                                        "}"
                                        "QGroupBox::title {"
                                        "subcontrol-origin: margin;"
                                        "left: 10px;"
                                        "padding: 0 5px 0 5px;"
                                        "}");

    QVBoxLayout *readingsLayout = new QVBoxLayout(currentReadingsGroup);

    pressureValue = new QLabel("--/--");
    pressureValue->setAlignment(Qt::AlignCenter);
    QFont pressureFont = pressureValue->font();
    pressureFont.setPointSize(24);
    pressureFont.setBold(true);
    pressureValue->setFont(pressureFont);

    QLabel *pressureUnit = new QLabel(TR("mm Hg"));
    pressureUnit->setAlignment(Qt::AlignCenter);

    pulseValue = new QLabel("--");
    pulseValue->setAlignment(Qt::AlignCenter);
    QFont pulseFont = pulseValue->font();
    pulseFont.setPointSize(24);
    pulseFont.setBold(true);
    pulseValue->setFont(pulseFont);

    pulseLabel = new QLabel(TR("Pulse"));
    pulseLabel->setAlignment(Qt::AlignCenter);

    QLabel *pulseUnit = new QLabel(TR("bpm"));
    pulseUnit->setAlignment(Qt::AlignCenter);

    readingsLayout->addWidget(pressureValue);
    readingsLayout->addWidget(pressureUnit);
    readingsLayout->addSpacing(20);
    readingsLayout->addWidget(pulseValue);
    readingsLayout->addWidget(pulseLabel);
    readingsLayout->addWidget(pulseUnit);

    leftColumnLayout->addWidget(currentReadingsGroup);

    // Measurement History - занимает всю оставшуюся высоту
    setupTable();
    tableGroup = new QGroupBox();
    tableGroup->setStyleSheet("QGroupBox {"
                              "border: 2px solid #cccccc;"
                              "border-radius: 10px;"
                              "margin-top: 1ex;"
                              "padding: 8px;"  // Уменьшил padding чтобы рамка не обрезалась
                              "}"
                              "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "padding: 0 5px 0 5px;"
                              "}");

    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setContentsMargins(2, 2, 2, 2); // Уменьшил margins для таблицы
    tableLayout->addWidget(dataTable);
    leftColumnLayout->addWidget(tableGroup);

    // Правая колонка - широкая (Indicators + Chart)
    QVBoxLayout *rightColumnLayout = new QVBoxLayout();
    rightColumnLayout->setSpacing(15);

    // Indicators
    indicatorsGroup = new QGroupBox();
    indicatorsGroup->setStyleSheet("QGroupBox {"
                                   "border: 2px solid #cccccc;"
                                   "border-radius: 10px;"
                                   "margin-top: 1ex;"
                                   "padding: 10px;"
                                   "}"
                                   "QGroupBox::title {"
                                   "subcontrol-origin: margin;"
                                   "left: 10px;"
                                   "padding: 0 5px 0 5px;"
                                   "}");

    QVBoxLayout *indicatorsLayout = new QVBoxLayout(indicatorsGroup);

    // Индикатор давления - ГОРИЗОНТАЛЬНЫЙ (все элементы справа)
    QHBoxLayout *pressureIndicatorLayout = new QHBoxLayout();

    pressureIndicator = new QWidget();
    pressureIndicator->setFixedSize(80, 12);
    // ДАВЛЕНИЕ: зеленый цвет
    pressureIndicator->setStyleSheet("background-color: #4CAF50; border-radius: 6px;");

    pressureChartBtn = new QPushButton("P");
    pressureChartBtn->setFixedSize(30, 30);
    pressureChartBtn->setCheckable(true);
    pressureChartBtn->setChecked(true);
    // ДАВЛЕНИЕ: зеленый цвет
    pressureChartBtn->setStyleSheet("QPushButton {"
                                    "background-color: #4CAF50;"
                                    "border-radius: 15px;"
                                    "border: 2px solid #388E3C;"
                                    "color: white;"
                                    "}"
                                    "QPushButton:checked {"
                                    "background-color: #4CAF50;"
                                    "}"
                                    "QPushButton:unchecked {"
                                    "background-color: #cccccc;"
                                    "}");

    // Добавляем растяжку слева, чтобы все элементы были справа
    pressureIndicatorLayout->addStretch();
    pressureIndicatorLayout->addWidget(pressureIndicator);
    pressureIndicatorLayout->addWidget(pressureChartBtn);

    // Индикатор пульса - ГОРИЗОНТАЛЬНЫЙ (все элементы справа)
    QHBoxLayout *pulseIndicatorLayout = new QHBoxLayout();

    pulseIndicator = new QWidget();
    pulseIndicator->setFixedSize(80, 12);
    // ПУЛЬС: красный цвет
    pulseIndicator->setStyleSheet("background-color: #f44336; border-radius: 6px;");

    pulseChartBtn = new QPushButton("H");
    pulseChartBtn->setFixedSize(30, 30);
    pulseChartBtn->setCheckable(true);
    pulseChartBtn->setChecked(true);
    // ПУЛЬС: красный цвет
    pulseChartBtn->setStyleSheet("QPushButton {"
                                 "background-color: #f44336;"
                                 "border-radius: 15px;"
                                 "border: 2px solid #d32f2f;"
                                 "color: white;"
                                 "}"
                                 "QPushButton:checked {"
                                 "background-color: #f44336;"
                                 "}"
                                 "QPushButton:unchecked {"
                                 "background-color: #cccccc;"
                                 "}");

    // Добавляем растяжку слева, чтобы все элементы были справа
    pulseIndicatorLayout->addStretch();
    pulseIndicatorLayout->addWidget(pulseIndicator);
    pulseIndicatorLayout->addWidget(pulseChartBtn);

    indicatorsLayout->addLayout(pressureIndicatorLayout);
    indicatorsLayout->addSpacing(10);
    indicatorsLayout->addLayout(pulseIndicatorLayout);

    rightColumnLayout->addWidget(indicatorsGroup);

    // Chart - занимает всю оставшуюся высоту
    setupChart();
    chartGroup = new QGroupBox();
    chartGroup->setStyleSheet("QGroupBox {"
                              "border: 2px solid #cccccc;"
                              "border-radius: 10px;"
                              "margin-top: 1ex;"
                              "padding: 10px;"
                              "}"
                              "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "padding: 0 5px 0 5px;"
                              "}");

    QVBoxLayout *chartLayout = new QVBoxLayout(chartGroup);
    chartLayout->addWidget(chartView);
    rightColumnLayout->addWidget(chartGroup);

    // Добавляем колонки в основной layout
    centerMainLayout->addLayout(leftColumnLayout, 1);  // Левая колонка - 1 часть
    centerMainLayout->addLayout(rightColumnLayout, 2); // Правая колонка - 2 части (в 2 раза шире)

    mainLayout->addWidget(centerContainer);

    // Подключение сигналов
    connect(addEntryBtn, &QPushButton::clicked, this, &MainWindow::addEntry);
    connect(accountBtn, &QPushButton::clicked, this, &MainWindow::showAccount);
    connect(supportBtn, &QPushButton::clicked, this, &MainWindow::showSupport);
    connect(pressureChartBtn, &QPushButton::toggled, this, &MainWindow::togglePressureChart);
    connect(pulseChartBtn, &QPushButton::toggled, this, &MainWindow::togglePulseChart);

    // Подключаем сигнал смены языка из нашего TranslationManager
    connect(&TranslationManager::instance(), &TranslationManager::languageChanged,
            this, &MainWindow::onLanguageChanged);

    retranslateUi();
}

void MainWindow::setupChart()
{
    chart = new QChart();
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    pressureSeries = new QLineSeries();
    pressureSeries->setName(TR("Pressure"));
    // ДАВЛЕНИЕ: зеленый цвет
    pressureSeries->setColor(QColor("#4CAF50"));
    pressureSeries->setPointsVisible(true);

    pulseSeries = new QLineSeries();
    pulseSeries->setName(TR("Pulse"));
    // ПУЛЬС: красный цвет
    pulseSeries->setColor(QColor("#f44336"));
    pulseSeries->setPointsVisible(true);

    chart->addSeries(pressureSeries);
    chart->addSeries(pulseSeries);

    axisX = new QValueAxis();
    axisX->setTitleText(TR("Days"));
    axisX->setLabelFormat("%d");
    axisX->setTickCount(10);

    axisYPressure = new QValueAxis();
    axisYPressure->setTitleText(TR("Pressure (mm Hg)"));
    axisYPressure->setRange(0, 200);
    // ДАВЛЕНИЕ: зеленый цвет
    axisYPressure->setLinePenColor(QColor("#4CAF50"));
    axisYPressure->setLabelsColor(QColor("#4CAF50"));

    axisYPulse = new QValueAxis();
    axisYPulse->setTitleText(TR("Pulse (bpm)"));
    axisYPulse->setRange(0, 150);
    // ПУЛЬС: красный цвет
    axisYPulse->setLinePenColor(QColor("#f44336"));
    axisYPulse->setLabelsColor(QColor("#f44336"));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisYPressure, Qt::AlignLeft);
    chart->addAxis(axisYPulse, Qt::AlignRight);

    pressureSeries->attachAxis(axisX);
    pressureSeries->attachAxis(axisYPressure);
    pulseSeries->attachAxis(axisX);
    pulseSeries->attachAxis(axisYPulse);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::setupTable()
{
    dataTable = new QTableWidget();
    dataTable->setColumnCount(4);
    dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    dataTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    // Стиль для таблицы с закругленными углами и исправленной рамкой
    dataTable->setStyleSheet("QTableWidget {"
                             "background-color: white;"
                             "border: 2px solid #cccccc;"
                             "border-radius: 8px;"  // Закругленные углы таблицы
                             "gridline-color: #e0e0e0;"
                             "}"
                             "QTableWidget::item {"
                             "padding: 5px;"
                             "border-bottom: 1px solid #f0f0f0;"
                             "}"
                             "QTableWidget::item:selected {"
                             "background-color: #e3f2fd;"
                             "color: black;"
                             "}"
                             "QHeaderView::section {"
                             "background-color: #f5f5f5;"
                             "padding: 8px;"
                             "border: none;"
                             "border-bottom: 2px solid #cccccc;"
                             "border-right: 1px solid #e0e0e0;"
                             "font-weight: bold;"
                             "}"
                             "QHeaderView::section:first {"
                             "border-top-left-radius: 6px;"  // Закругление левого верхнего угла
                             "}"
                             "QHeaderView::section:last {"
                             "border-right: none;"
                             "border-top-right-radius: 6px;"  // Закругление правого верхнего угла
                             "}"
                             "QTableCornerButton::section {"
                             "background-color: #f5f5f5;"  // Закрашиваем левый верхний угол серым
                             "border-top: 2px solid #cccccc;"
                             "border-right: 1px solid #e0e0e0;"
                             "border-bottom: 2px solid #cccccc;"
                             "border-top-left-radius: 6px;"  // Закругление угла
                             "}");

    // Используем TR() для заголовков
    QStringList headers;
    headers << TR("Date") << TR("Systolic") << TR("Diastolic") << TR("Pulse");
    dataTable->setHorizontalHeaderLabels(headers);

    // Настраиваем растяжение колонок
    dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Устанавливаем минимальную высоту для заголовков
    dataTable->horizontalHeader()->setMinimumHeight(40);

    connect(dataTable, &QTableWidget::itemChanged, this, &MainWindow::onTableItemChanged);
}

void MainWindow::loadData()
{
    QSettings settings;
    int size = settings.beginReadArray("pressureData");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        PressureData data;
        data.date = QDate::fromString(settings.value("date").toString(), "yyyy-MM-dd");
        data.systolic = settings.value("systolic").toInt();
        data.diastolic = settings.value("diastolic").toInt();
        data.pulse = settings.value("pulse").toInt();
        pressureData.append(data);
    }

    settings.endArray();

    // Обновление таблицы
    dataTable->setRowCount(pressureData.size());
    for (int i = 0; i < pressureData.size(); ++i) {
        const PressureData &data = pressureData[i];

        QTableWidgetItem *dateItem = new QTableWidgetItem(data.date.toString("yyyy-MM-dd"));
        QTableWidgetItem *systolicItem = new QTableWidgetItem(QString::number(data.systolic));
        QTableWidgetItem *diastolicItem = new QTableWidgetItem(QString::number(data.diastolic));
        QTableWidgetItem *pulseItem = new QTableWidgetItem(QString::number(data.pulse));

        dataTable->setItem(i, 0, dateItem);
        dataTable->setItem(i, 1, systolicItem);
        dataTable->setItem(i, 2, diastolicItem);
        dataTable->setItem(i, 3, pulseItem);
    }
}

void MainWindow::saveData()
{
    QSettings settings;
    settings.beginWriteArray("pressureData", pressureData.size());

    for (int i = 0; i < pressureData.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("date", pressureData[i].date.toString("yyyy-MM-dd"));
        settings.setValue("systolic", pressureData[i].systolic);
        settings.setValue("diastolic", pressureData[i].diastolic);
        settings.setValue("pulse", pressureData[i].pulse);
    }

    settings.endArray();
}

void MainWindow::updateCurrentReadings()
{
    if (pressureData.isEmpty()) {
        pressureValue->setText("--/--");
        pulseValue->setText("--");
        return;
    }

    const PressureData &latest = pressureData.last();
    pressureValue->setText(QString("%1/%2").arg(latest.systolic).arg(latest.diastolic));
    pulseValue->setText(QString::number(latest.pulse));
}

void MainWindow::updateChart()
{
    pressureSeries->clear();
    pulseSeries->clear();

    if (pressureData.isEmpty()) return;

    // Сортируем по дате
    std::sort(pressureData.begin(), pressureData.end(),
              [](const PressureData &a, const PressureData &b) { return a.date < b.date; });

    QDate startDate = pressureData.first().date;

    for (int i = 0; i < pressureData.size(); ++i) {
        const PressureData &data = pressureData[i];
        int days = startDate.daysTo(data.date);

        // Среднее давление для графика
        double avgPressure = (data.systolic + data.diastolic) / 2.0;
        pressureSeries->append(days, avgPressure);
        pulseSeries->append(days, data.pulse);
    }

    // Обновляем оси
    if (!pressureData.isEmpty()) {
        int maxDays = startDate.daysTo(pressureData.last().date);
        axisX->setRange(0, qMax(maxDays, 1));
    }
}

void MainWindow::addEntry()
{
    AddEntryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        PressureData newData = dialog.getData();
        pressureData.append(newData);

        // Добавляем в таблицу
        int row = dataTable->rowCount();
        dataTable->insertRow(row);

        dataTable->setItem(row, 0, new QTableWidgetItem(newData.date.toString("yyyy-MM-dd")));
        dataTable->setItem(row, 1, new QTableWidgetItem(QString::number(newData.systolic)));
        dataTable->setItem(row, 2, new QTableWidgetItem(QString::number(newData.diastolic)));
        dataTable->setItem(row, 3, new QTableWidgetItem(QString::number(newData.pulse)));

        updateChart();
        updateCurrentReadings();
        saveData();
    }
}

void MainWindow::showAccount()
{
    AccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Обновляем язык через нашу систему переводов
        QSettings settings;
        QString language = settings.value("language", "en").toString();
        TranslationManager::instance().setLanguage(language);

        QMessageBox::information(this, TR("Settings Updated"),
                                 TR("Account settings have been updated successfully."));
    }
}

void MainWindow::showSupport()
{
    SupportDialog dialog(this);
    dialog.exec();
}

void MainWindow::onTableItemChanged(QTableWidgetItem *item)
{
    int row = item->row();
    int column = item->column();

    if (row >= 0 && row < pressureData.size()) {
        PressureData &data = pressureData[row];

        switch (column) {
        case 0: // Date
            data.date = QDate::fromString(item->text(), "yyyy-MM-dd");
            break;
        case 1: // Systolic
            data.systolic = item->text().toInt();
            break;
        case 2: // Diastolic
            data.diastolic = item->text().toInt();
            break;
        case 3: // Pulse
            data.pulse = item->text().toInt();
            break;
        }

        updateChart();
        updateCurrentReadings();
        saveData();
    }
}

void MainWindow::togglePressureChart(bool visible)
{
    pressureSeries->setVisible(visible);
    axisYPressure->setVisible(visible);
}

void MainWindow::togglePulseChart(bool visible)
{
    pulseSeries->setVisible(visible);
    axisYPulse->setVisible(visible);
}

// НОВЫЙ МЕТОД: Обработчик смены языка
void MainWindow::onLanguageChanged()
{
    retranslateUi();
}

void MainWindow::retranslateUi()
{
    setWindowTitle(TR("PressureTracker"));
    appTitle->setText(TR("PressureTracker"));
    pulseLabel->setText(TR("Pulse"));

    currentReadingsGroup->setTitle(TR("Current Readings"));
    indicatorsGroup->setTitle(TR("Indicators"));
    chartGroup->setTitle(TR("Pressure and Pulse Chart"));
    tableGroup->setTitle(TR("Measurement History"));

    addEntryBtn->setToolTip(TR("Add new measurement"));
    accountBtn->setToolTip(TR("My Account"));
    supportBtn->setToolTip(TR("Support Developer"));

    pressureChartBtn->setToolTip(TR("Show/Hide Pressure Chart"));
    pulseChartBtn->setToolTip(TR("Show/Hide Pulse Chart"));

    axisX->setTitleText(TR("Days"));
    axisYPressure->setTitleText(TR("Pressure (mm Hg)"));
    axisYPulse->setTitleText(TR("Pulse (bpm)"));

    // Обновляем заголовки таблицы
    QStringList headers;
    headers << TR("Date") << TR("Systolic") << TR("Diastolic") << TR("Pulse");
    dataTable->setHorizontalHeaderLabels(headers);

    // Обновляем названия серий на графике
    pressureSeries->setName(TR("Pressure"));
    pulseSeries->setName(TR("Pulse"));
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QMainWindow::changeEvent(event);
}
