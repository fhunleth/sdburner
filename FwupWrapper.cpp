#include "FwupWrapper.h"
#include <QProcess>
#include <QTimer>
#include <QtEndian>


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
    QStringList args;

#ifndef HOST_DEBUGGING
    fwup_->setProgram("fwup");
#else
    fwup_->setProgram("gksudo");
    args << "--"
         << "fwup";
    useAutodetectedCard_ = false;
    destinationPath_ = "/tmp/foo";
#endif

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

int FwupWrapper::processInput()
{
    const uchar *inputData = (const uchar *) inputBuffer_.constData();
    quint32 len = qFromBigEndian<quint32>(inputData);

    if (len > 1000) {
        qCritical("Bad length in fwup input: %d", len);
        fwup_->kill();
        return inputBuffer_.count();
    }

    len += 4; // Add the count field.

    if (inputBuffer_.count() < (int) len)
        return -1;

    char messageType[2];
    messageType[0] = inputBuffer_.at(4);
    messageType[1] = inputBuffer_.at(5);

    if (messageType[0] == 'O' && messageType[1] == 'K')
        emit completed();
    else if (messageType[0] == 'E' && messageType[1] == 'R' && len >= 8)
        emit error(QString::fromLatin1(inputBuffer_.mid(8, len - 8)));
    else if (messageType[0] == 'W' && messageType[1] == 'N' && len >= 8)
        emit warning(QString::fromLatin1(inputBuffer_.mid(8, len - 8)));
    else if (messageType[0] == 'P' && messageType[1] == 'R' && len >= 8)
        emit progress(qFromBigEndian<qint16>(inputData + 6));
    else {
        qCritical("Received unexpected message from fwup: %c%c", messageType[0], messageType[1]);
        fwup_->kill();
        return inputBuffer_.count();
    }

    return len;
}

void FwupWrapper::fwupFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qCritical("fwup is done. exitCode=%d, status=%d", exitCode, exitStatus);
    fwup_->deleteLater();
    fwup_ = 0;
}

void FwupWrapper::cancel()
{
    qCritical("Cancelling fwup operation!");
    fwup_->kill();
}
