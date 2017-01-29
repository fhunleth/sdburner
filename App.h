#ifndef APP_H
#define APP_H

#include <QObject>

class FwupWrapper;
class UiWidget;
class QTimer;

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
    void handleSdcardPollTimeout();

    void progress(int percent);
    void error(QString reason);
    void warning(QString reason);
    void completed();

    void handleUserSaysGo();
    void handleUserSaysNoGo();

private:
    enum State {
        WAITING_FOR_SDCARD,
        WAITING_FOR_START,
        PROGRAMMING,
        WAITING_FOR_SDCARD_REMOVAL
    };

    enum Event {
        ENTER_STATE,
        EXIT_STATE,
        SDCARD_INSERTED,
        SDCARD_REMOVED,
        USER_SAYS_GO,
        USER_SAYS_NO_GO,
        PROGRAMMING_ERROR,
        PROGRAMMING_OK,
    };

    void handleEvent(Event event);

    void handleWaitingForSdcard(Event event);
    void handleWaitingForStart(Event event);
    void handleProgramming(Event event);
    void handleWaitingForSdcardRemoval(Event event);

    void gotoState(State state);
private:

    FwupWrapper *fwup_;
    UiWidget *ui_;
    State state_;

    QTimer *sdcardPoller_;
};

#endif // APP_H
