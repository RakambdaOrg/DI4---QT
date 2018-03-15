#-------------------------------------------------
#
# Project created by QtCreator 2018-02-13T10:40:23
#
#-------------------------------------------------

QT       += core gui \
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Introduction
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
    c_init_bd.cpp \
    AboutDialog.cpp \
    DBConnect.cpp \
    Login.cpp \
    MainWindow.cpp \
    MyDateItemDelegate.cpp \
    MySqlTableModel.cpp \
    NewCustomerDialog.cpp \
    NewStaffDialog.cpp \
    Customer.cpp \
    RessourceType.cpp \
    Schedule.cpp \
    Staff.cpp \
    StaffItem.cpp \
    Timeline.cpp \
    treeitem.cpp \
    treemodel.cpp

HEADERS  += \
    c_init_bd.h \
    AboutDialog.h \
    DBConnect.h \
    Login.h \
    MainWindow.h \
    MyDateItemDelegate.h \
    MySqlTableModel.h \
    NewCustomerDialog.h \
    NewDtaffDialog.h \
    Customer.h \
    RessourceType.h \
    Schedule.h \
    Staff.h \
    StaffItem.h \
    Timeline.h \
    treeitem.h \
    treemodel.h

FORMS    += \
    AboutDialog.ui \
    Login.ui \
    MainWindow.ui \
    NewCustomerDialog.ui \
    NewStaffDialog.ui

RESOURCES += \
    resources.qrc
