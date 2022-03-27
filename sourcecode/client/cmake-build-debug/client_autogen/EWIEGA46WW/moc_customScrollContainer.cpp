/****************************************************************************
** Meta object code from reading C++ file 'customScrollContainer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../customScrollContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customScrollContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ScrollAreaCustom_t {
    const uint offsetsAndSize[12];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ScrollAreaCustom_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ScrollAreaCustom_t qt_meta_stringdata_ScrollAreaCustom = {
    {
QT_MOC_LITERAL(0, 16), // "ScrollAreaCustom"
QT_MOC_LITERAL(17, 15), // "scrollContainer"
QT_MOC_LITERAL(33, 0), // ""
QT_MOC_LITERAL(34, 9), // "updateSpd"
QT_MOC_LITERAL(44, 15), // "scrollIndicator"
QT_MOC_LITERAL(60, 2) // "dp"

    },
    "ScrollAreaCustom\0scrollContainer\0\0"
    "updateSpd\0scrollIndicator\0dp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScrollAreaCustom[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x08,    1 /* Private */,
       3,    0,   33,    2, 0x08,    2 /* Private */,
       4,    1,   34,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void ScrollAreaCustom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScrollAreaCustom *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->scrollContainer(); break;
        case 1: _t->updateSpd(); break;
        case 2: _t->scrollIndicator((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ScrollAreaCustom::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ScrollAreaCustom.offsetsAndSize,
    qt_meta_data_ScrollAreaCustom,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ScrollAreaCustom_t
, QtPrivate::TypeAndForceComplete<ScrollAreaCustom, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *ScrollAreaCustom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScrollAreaCustom::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScrollAreaCustom.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScrollAreaCustom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_ScrollListContainer_t {
    const uint offsetsAndSize[2];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ScrollListContainer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ScrollListContainer_t qt_meta_stringdata_ScrollListContainer = {
    {
QT_MOC_LITERAL(0, 19) // "ScrollListContainer"

    },
    "ScrollListContainer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScrollListContainer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ScrollListContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject ScrollListContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ScrollListContainer.offsetsAndSize,
    qt_meta_data_ScrollListContainer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ScrollListContainer_t
, QtPrivate::TypeAndForceComplete<ScrollListContainer, std::true_type>



>,
    nullptr
} };


const QMetaObject *ScrollListContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScrollListContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScrollListContainer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScrollListContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_ScrollIndicator_t {
    const uint offsetsAndSize[6];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ScrollIndicator_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ScrollIndicator_t qt_meta_stringdata_ScrollIndicator = {
    {
QT_MOC_LITERAL(0, 15), // "ScrollIndicator"
QT_MOC_LITERAL(16, 10), // "scrollPage"
QT_MOC_LITERAL(27, 0) // ""

    },
    "ScrollIndicator\0scrollPage\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScrollIndicator[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void ScrollIndicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScrollIndicator *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->scrollPage((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ScrollIndicator::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScrollIndicator::scrollPage)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ScrollIndicator::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ScrollIndicator.offsetsAndSize,
    qt_meta_data_ScrollIndicator,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ScrollIndicator_t
, QtPrivate::TypeAndForceComplete<ScrollIndicator, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>



>,
    nullptr
} };


const QMetaObject *ScrollIndicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScrollIndicator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScrollIndicator.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScrollIndicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ScrollIndicator::scrollPage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
