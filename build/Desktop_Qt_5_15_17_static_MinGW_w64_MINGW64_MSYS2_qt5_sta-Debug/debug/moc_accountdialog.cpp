/****************************************************************************
** Meta object code from reading C++ file 'accountdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../accountdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'accountdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AccountDialog_t {
    QByteArrayData data[11];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AccountDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AccountDialog_t qt_meta_stringdata_AccountDialog = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AccountDialog"
QT_MOC_LITERAL(1, 14, 12), // "saveSettings"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "logout"
QT_MOC_LITERAL(4, 35, 16), // "enableSaveButton"
QT_MOC_LITERAL(5, 52, 13), // "retranslateUi"
QT_MOC_LITERAL(6, 66, 17), // "onLanguageChanged"
QT_MOC_LITERAL(7, 84, 14), // "changePassword"
QT_MOC_LITERAL(8, 99, 22), // "validatePasswordFields"
QT_MOC_LITERAL(9, 122, 11), // "changePhoto"
QT_MOC_LITERAL(10, 134, 18) // "loadAvatarFromFile"

    },
    "AccountDialog\0saveSettings\0\0logout\0"
    "enableSaveButton\0retranslateUi\0"
    "onLanguageChanged\0changePassword\0"
    "validatePasswordFields\0changePhoto\0"
    "loadAvatarFromFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AccountDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AccountDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AccountDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->saveSettings(); break;
        case 1: _t->logout(); break;
        case 2: _t->enableSaveButton(); break;
        case 3: _t->retranslateUi(); break;
        case 4: _t->onLanguageChanged(); break;
        case 5: _t->changePassword(); break;
        case 6: _t->validatePasswordFields(); break;
        case 7: _t->changePhoto(); break;
        case 8: _t->loadAvatarFromFile(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject AccountDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_AccountDialog.data,
    qt_meta_data_AccountDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AccountDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AccountDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AccountDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int AccountDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
