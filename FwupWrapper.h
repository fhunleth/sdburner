#ifndef FWUPWRAPPER_H
#define FWUPWRAPPER_H

#include <QObject>
#include <QProcess>

class QTimer;

class FwupWrapper : public QObject
{
    Q_OBJECT
public:
    explicit FwupWrapper(QObject *parent = 0);

    void setInput(const QString &path) { inputPath_ = path; }
    QString input() const { return inputPath_; }

    void setDestination(const QString &path) { destinationPath_ = path; }
    QString destination() const { return destinationPath_; }

    void setTask(const QString &name) { task_ = name; }
    QString task() const { return task_; }

    void apply();

    QString version() const;

    void startDetection();
    void stopDetection();

    QStringList detectMemoryCards() const;
signals:
    void memoryCardsDetected(QStringList paths);

    void progress(int percent);
    void error(QString reason);
    void completed();

public slots:
    void cancel();

private slots:
    void fwupReadReady();
    void fwupFinished(int,QProcess::ExitStatus);
    void detectionTimeout();

private:
    QProcess *fwup_;

    QString inputPath_;
    QString destinationPath_;
    QString task_;

    QTimer *detectionTimer_;
    QStringList currentlyDetectedCards_;
};

#endif // FWUPWRAPPER_H
