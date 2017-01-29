#include "FwupWrapper.h"
#include <QProcess>
#include <QTimer>

FwupWrapper::FwupWrapper(QObject *parent) : QObject(parent),
    fwup_(0),
    task_("complete")
{
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

    if (useAutodetectedCard_) {
        args << "-y";
    } else {
        if (destinationPath_.isEmpty())
            qFatal("Either autodetect or specify a destination");
        args << "-d"
             << destinationPath_;
    }

    fwup_->setArguments(args);
    connect(fwup_, SIGNAL(readyRead()), SLOT(fwupReadReady()));
    connect(fwup_, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(fwupFinished(int,QProcess::ExitStatus)));

    inputBuffer_.clear();
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

QList<FwupWrapper::MemoryCard> FwupWrapper::detectMemoryCards() const
{
    QList<MemoryCard> cards;

    QProcess p;
    p.setProgram("fwup");
    p.setArguments(QStringList() << "--detect");
    p.start(QProcess::ReadOnly);
    if (!p.waitForFinished(1000)) {
        qCritical("fwup call to get version timed out");
        return cards;
    }

    QStringList cardsizes =
            QString::fromLatin1(p.readAll()).split(QLatin1Char('\n'), QString::SkipEmptyParts);

    foreach (QString cardCommaSize, cardsizes) {
        QStringList cardSize = cardCommaSize.split(QLatin1Char(','));
        if (cardSize.count() == 2)
            cards << MemoryCard(cardSize.at(0),
                                cardSize.at(1).toLongLong());
    }

    return cards;
}

void FwupWrapper::fwupReadReady()
{
    inputBuffer_ += fwup_->readAll();

    while (inputBuffer_.count() >= 4) {
        int amountProcessed = processInput();
        if (amountProcessed > 0)
            inputBuffer_ = inputBuffer_.mid(amountProcessed);
        else
            break;
    }
}

void FwupWrapper::fwupFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QDataStream ds;

}

void FwupWrapper::cancel()
{

}
