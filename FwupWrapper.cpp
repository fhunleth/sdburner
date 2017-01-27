#include "FwupWrapper.h"
#include <QProcess>
#include <QTimer>

FwupWrapper::FwupWrapper(QObject *parent) : QObject(parent),
    fwup_(0),
    task_("complete")
{
    detectionTimer_ = new QTimer(this);
    connect(detectionTimer_, SIGNAL(timeout()), SLOT(detectionTimeout()));
}

void FwupWrapper::apply()
{
    if (fwup_)
        qFatal("Existing fwup call");

    fwup_ = new QProcess(this);
    fwup_->setProgram("fwup");
    QStringList args;
    args << "--framing"
         << "--apply"
         << "-i" << inputPath_
         << "--task" << task_;
    fwup_->setArguments(args);
    connect(fwup_, SIGNAL(readyRead()), SLOT(fwupReadReady()));
    connect(fwup_, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(fwupFinished(int,QProcess::ExitStatus)));

    fwup_->start(QProcess::ReadOnly);
}

QString FwupWrapper::version() const
{
    QProcess p;
    p.setProgram("fwup");
    p.setArguments(QStringList() << "--version");
    p.start(QProcess::ReadOnly);
    if (!p.waitForFinished(1000)) {
        qCritical("fwup call to get version timed out");
        return QString();
    }
    return QString::fromLatin1(p.readAll());
}

QStringList FwupWrapper::detectMemoryCards() const
{
    QProcess p;
    p.setProgram("fwup");
    p.setArguments(QStringList() << "--detect");
    p.start(QProcess::ReadOnly);
    if (!p.waitForFinished(1000)) {
        qCritical("fwup call to get version timed out");
        return QStringList();
    }
    return QString::fromLatin1(p.readAll()).split(QLatin1Char('\n'));
}

void FwupWrapper::fwupReadReady()
{

}

void FwupWrapper::fwupFinished(int exitCode, QProcess::ExitStatus exitStatus)
{

}

void FwupWrapper::detectionTimeout()
{
    QStringList detected = detectMemoryCards();

    if (detected != currentlyDetectedCards_) {
        currentlyDetectedCards_ = detected;
        emit memoryCardsDetected(currentlyDetectedCards_);
    }
}

void FwupWrapper::cancel()
{

}
