#include "App.h"
#include "UiWidget.h"
#include "FwupWrapper.h"

App::App(QObject *parent) : QObject(parent)
{
    ui_ = new UiWidget();
    fwup_ = new FwupWrapper(this);
    connect(fwup_, SIGNAL(progress(int)), SLOT(progress(int)));
    connect(fwup_, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(fwup_, SIGNAL(warning(QString)), SLOT(warning(QString)));
    connect(fwup_, SIGNAL(completed()), SLOT(completed()));
}

App::~App()
{
    delete ui_;
}

void App::start()
{
    ui_->show();
    fwup_->setInput("/home/fhunleth/raspberrypi/dexter_industries_jessie-2016.12.04.fw");
    fwup_->apply();
}

void App::progress(int percent)
{
    qDebug("progress %d", percent);
}

void App::error(QString reason)
{
    qDebug("error %s", qPrintable(reason));
}

void App::warning(QString reason)
{
    qDebug("warning %s", qPrintable(reason));
}

void App::completed()
{
    qDebug("completed");
}
