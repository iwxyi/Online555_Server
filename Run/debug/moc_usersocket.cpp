/****************************************************************************
** Meta object code from reading C++ file 'usersocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../usersocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usersocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UserSocket_t {
    QByteArrayData data[7];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserSocket_t qt_meta_stringdata_UserSocket = {
    {
QT_MOC_LITERAL(0, 0, 10), // "UserSocket"
QT_MOC_LITERAL(1, 11, 15), // "signalReadyRead"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "index"
QT_MOC_LITERAL(4, 34, 19), // "signalDisConnection"
QT_MOC_LITERAL(5, 54, 13), // "slotReadyRead"
QT_MOC_LITERAL(6, 68, 17) // "slotDisConnection"

    },
    "UserSocket\0signalReadyRead\0\0index\0"
    "signalDisConnection\0slotReadyRead\0"
    "slotDisConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserSocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   40,    2, 0x0a /* Public */,
       6,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UserSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UserSocket *_t = static_cast<UserSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalReadyRead((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signalDisConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotReadyRead(); break;
        case 3: _t->slotDisConnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UserSocket::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserSocket::signalReadyRead)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (UserSocket::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UserSocket::signalDisConnection)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject UserSocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_UserSocket.data,
      qt_meta_data_UserSocket,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UserSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UserSocket.stringdata0))
        return static_cast<void*>(const_cast< UserSocket*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int UserSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UserSocket::signalReadyRead(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserSocket::signalDisConnection(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
