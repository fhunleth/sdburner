QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sdburner
TEMPLATE = app

SOURCES += main.cpp\
    FwupWrapper.cpp \
    UiWidget.cpp \
    App.cpp

HEADERS  += \
    FwupWrapper.h \
    UiWidget.h \
    App.h

RESOURCES += \
    ui.qrc

target.path = /usr/bin
INSTALLS += target

unix:!cross_compile {
    QMAKE_CXXFLAGS += -DHOST_DEBUGGING
}
