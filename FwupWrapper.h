#ifndef FWUPWRAPPER_H
#define FWUPWRAPPER_H

#include <QObject>
#include <QProcess>

class QTimer;

class FwupWrapper : public QObject
{
    Q_OBJECT
public:
    struct MemoryCard {
        MemoryCard(QString where, qint64 howbig) : path(where), size(howbig) {}

        QString path;
        qint64 size;
    };

    explicit FwupWrapper(QObject *parent = 0);

    void setInput(const QString &path) { inputPath_ = path; }
    QString input() const { return inputPath_; }

    void setDestination(const QString &path) { destinationPath_ = path; }
    QString destination() const { return destinationPath_; }

    void setTask(const QString &name) { task_ = name; }
    QString task() const { return task_; }

    void setUseAutodetectedCard(bool yes) { useAutodetectedCard_ = yes; }
    bool useAutodetectedCard() const { return useAutodetectedCard_; }

    void apply();

    QString version() const;

    void startDetection();
    void stopDetection();

    QList<MemoryCard> detectMemoryCards() const;
signals:
    void progress(int percent);
    void error(QString reason);
    void warning(QString reason);
    void completed();

public slots:
    void cancel();

private slots:
    void fwupReadReady();
    void fwupFinished(int,QProcess::ExitStatus);

private:
    int processInput();

private:
    QProcess *fwup_;

    QString inputPath_;
    QString destinationPath_;
    QString task_;
    bool useAutodetectedCard_;

    QByteArray inputBuffer_;
};

#endif // FWUPWRAPPER_H
