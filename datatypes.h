#ifndef DATATYPES_H
#define DATATYPES_H

#include <QDate>

struct PressureData {
    QDate date;
    int systolic;
    int diastolic;
    int pulse;
};

#endif // DATATYPES_H
