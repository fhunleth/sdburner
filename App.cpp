#include "App.h"
#include "UiWidget.h"
#include "FwupWrapper.h"

#include <QTimer>

App::App(QObject *parent) : QObject(parent),
    state_(WAITING_FOR_SDCARD)
{
    ui_ = new UiWidget();
    connect(ui_, SIGNAL(userSaysGo()), SLOT(handleUserSaysGo()));
    connect(ui_, SIGNAL(userSaysNoGo()), SLOT(handleUserSaysNoGo()));

    fwup_ = new FwupWrapper(this);
    connect(fwup_, SIGNAL(progress(int)), SLOT(progress(int)));
    connect(fwup_, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(fwup_, SIGNAL(warning(QString)), SLOT(warning(QString)));
    connect(fwup_, SIGNAL(completed()), SLOT(completed()));

#ifdef HOST_DEBUGGING
    fwup_->setInput("/home/fhunleth/raspberrypi/sdcard.fw");
#else
    fwup_->setInput("/mnt/sdcard.fw");
#endif

    sdcardPoller_ = new QTimer(this);
    sdcardPoller_->setInterval(1000);
    connect(sdcardPoller_, SIGNAL(timeout()), SLOT(handleSdcardPollTimeout()));
}

App::~App()
{
    delete ui_;
}

void App::start()
{
    ui_->show();

#if 1
    qDebug("fwup version=%s", qPrintable(fwup_->version()));

    QList<FwupWrapper::MemoryCard> cards = fwup_->detectMemoryCards();
    foreach (FwupWrapper::MemoryCard card, cards) {
        qDebug("Detected: %s", qPrintable(card.path));
    }
#endif

    state_ = WAITING_FOR_SDCARD;
    handleEvent(ENTER_STATE);
}

void App::handleSdcardPollTimeout()
{
    QList<FwupWrapper::MemoryCard> cards = fwup_->detectMemoryCards();
    if (cards.isEmpty())
        handleEvent(SDCARD_REMOVED);
    else
        handleEvent(SDCARD_INSERTED);
}

void App::progress(int percent)
{
    qDebug("progress %d", percent);
    ui_->setProgress(percent);
}

void App::error(QString reason)
{
    qDebug("error %s", qPrintable(reason));
    handleEvent(PROGRAMMING_ERROR);
}

void App::warning(QString reason)
{
    qDebug("warning %s", qPrintable(reason));
}

void App::completed()
{
    qDebug("completed");
    handleEvent(PROGRAMMING_OK);
}

void App::handleUserSaysGo()
{
    qDebug("user says go!");
    handleEvent(USER_SAYS_GO);
}

void App::handleUserSaysNoGo()
{
    qDebug("user says no go!");
    handleEvent(USER_SAYS_NO_GO);
}

void App::handleEvent(App::Event event)
{
    switch (state_) {
    case WAITING_FOR_SDCARD:
        handleWaitingForSdcard(event);
        break;
    case WAITING_FOR_START:
        handleWaitingForStart(event);
        break;
    case PROGRAMMING:
        handleProgramming(event);
        break;
    case WAITING_FOR_SDCARD_REMOVAL:
        handleWaitingForSdcardRemoval(event);
        break;
    }
}

void App::gotoState(App::State state)
{
    if (state != state_) {
        handleEvent(EXIT_STATE);
        state_ = state;
        handleEvent(ENTER_STATE);
    }
}

void App::handleWaitingForSdcard(App::Event event)
{
    switch (event) {
    case ENTER_STATE:
        ui_->showScreen(UiWidget::INSERT_SDCARD);
        sdcardPoller_->start();
        break;

    case EXIT_STATE:
        sdcardPoller_->stop();
        break;

    case SDCARD_INSERTED:
        gotoState(WAITING_FOR_START);
        break;

    default:
        break;
    }
}

void App::handleWaitingForStart(App::Event event)
{
    switch (event) {
    case ENTER_STATE:
        sdcardPoller_->start();
        ui_->showScreen(UiWidget::ASK_USER);
        break;

    case EXIT_STATE:
        sdcardPoller_->stop();
        break;

    case SDCARD_REMOVED:
        gotoState(WAITING_FOR_SDCARD);
        break;

    case USER_SAYS_GO:
        gotoState(PROGRAMMING);
        break;

    case USER_SAYS_NO_GO:
        ui_->showScreen(UiWidget::PROGRAMMING_ERROR); // Weird, but this is the current screen.
        gotoState(WAITING_FOR_SDCARD_REMOVAL);
        break;

    default:
        break;
    }
}

void App::handleProgramming(Event event)
{
    switch (event) {
    case ENTER_STATE:
        ui_->setProgress(0);
        ui_->showScreen(UiWidget::PROGRAMMING);
        fwup_->apply();
        break;

    case EXIT_STATE:
        break;

    case PROGRAMMING_ERROR:
        ui_->showScreen(UiWidget::PROGRAMMING_ERROR);
        gotoState(WAITING_FOR_SDCARD_REMOVAL);
        break;

    case PROGRAMMING_OK:
        ui_->showScreen(UiWidget::PROGRAMMING_SUCCESS);
        gotoState(WAITING_FOR_SDCARD_REMOVAL);
        break;

    default:
        break;
    }
}

void App::handleWaitingForSdcardRemoval(App::Event event)
{
    switch (event) {
    case ENTER_STATE:
        sdcardPoller_->start();
        break;

    case EXIT_STATE:
        sdcardPoller_->stop();
        break;

    case SDCARD_REMOVED:
        gotoState(WAITING_FOR_SDCARD);
        break;

    default:
        break;
    }
}

