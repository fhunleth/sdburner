#ifndef APP_H
#define APP_H

#include <QObject>

class FwupWrapper;
class UiWidget;

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);
    ~App();

    void start();

signals:

public slots:

private slots:
    void progress(int percent);
    void error(QString reason);
    void warning(QString reason);
    void completed();

private:
    FwupWrapper *fwup_;
    UiWidget *ui_;
};

#endif // APP_H
