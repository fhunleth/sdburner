QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sdburner
TEMPLATE = app

SOURCES += main.cpp\
    MainForm.cpp \
    FwupWrapper.cpp \
    UiWidget.cpp \
    App.cpp

HEADERS  += \
    MainForm.h \
    FwupWrapper.h \
    UiWidget.h \
    App.h

FORMS    += \
    MainForm.ui

target.path = /usr/bin
INSTALLS += target

RESOURCES += \
    ui.qrc

STATECHARTS +=
