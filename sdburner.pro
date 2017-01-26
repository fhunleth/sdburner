QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sdburner
TEMPLATE = app

SOURCES += main.cpp\
    MainForm.cpp

HEADERS  += \
    MainForm.h

FORMS    += \
    MainForm.ui

target.path = /usr/bin
INSTALLS += target
