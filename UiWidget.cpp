#include "UiWidget.h"
#include <QPainter>
#include <QKeyEvent>

const UiWidget::ScreenInfo UiWidget::screens_[UiWidget::NUM_SCREENS] = {
    /* INSERT_SDCARD */ { ":/ui/memcard.svg.png", false, &UiWidget::handleInsertSdcardButton },
    /* ASK_USER */ { ":/ui/programornot.svg.png", false, &UiWidget::handleAskUserButton },
    /* PROGRAMMING */ { ":/ui/programming.svg.png", true, 0 },
    /* PROGRAMMING_ERROR */ { ":/ui/remvsdcard.svg.png", false, 0 },
    /* PROGRAMMING_SUCCESS */ { ":/ui/finished.svg.png", false, 0 },
    /* ABOUT */ { ":/ui/about.svg.png", false, &UiWidget::handleAboutScreenButton },
    /* FUN1 */ { ":/ui/fun.svg.png", false, &UiWidget::handleFunScreenButton },
    /* FUN2 */ { ":/ui/fun1_5.svg.png", false, &UiWidget::handleFunScreenButton },
    /* FUN3 */ { ":/ui/fun2.svg.png", false, &UiWidget::handleFunScreenButton },
};

UiWidget::UiWidget(QWidget *parent) : QWidget(parent)
{
    resize(320, 240);

    currentScreen_ = INSERT_SDCARD;

    for (int i = 0; i < NUM_SCREENS; i++)
        screenImages_[i].load(screens_[i].imageFile);
}

void UiWidget::showScreen(UiWidget::Screen screen)
{
    if (screen < 0 || screen >= NUM_SCREENS)
        qFatal("Bad screen: %d", screen);

    currentScreen_ = screen;
    update();
}

void UiWidget::setProgress(int progress)
{
    progress_ = progress;
    update();
}

void UiWidget::keyPressEvent(QKeyEvent *event)
{
    const ScreenInfo *info = &screens_[currentScreen_];

    if (info->handleButton)
        (this->*(info->handleButton))(event->key());
}

void UiWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.drawImage(0, 0, screenImages_[currentScreen_]);

    if (screens_[currentScreen_].showProgress) {
        p.setFont(QFont("Sans", 30));
        p.drawText(QRect(0, 60, 320, 50), Qt::AlignHCenter, tr("%1%").arg(progress_));
    }
}

void UiWidget::handleInsertSdcardButton(int button)
{
    switch (button) {
    case Qt::Key_A:
        currentScreen_ = ABOUT;
        break;
    case Qt::Key_B:
        currentScreen_ = FUN1;
        break;
    default:
        break;
    }
    update();
}

void UiWidget::handleFunScreenButton(int button)
{
    if (currentScreen_ == FUN1 && button == Qt::Key_D)
        currentScreen_ = FUN2;
    else if (currentScreen_ == FUN2 && button == Qt::Key_D)
        currentScreen_ = FUN3;
    else if (currentScreen_ == FUN3 && button == Qt::Key_D)
        currentScreen_ = INSERT_SDCARD;
    update();
}

void UiWidget::handleAboutScreenButton(int button)
{
    if (button == Qt::Key_D) {
        currentScreen_ = INSERT_SDCARD;
        update();
    }
}

void UiWidget::handleAskUserButton(int button)
{
    if (button == Qt::Key_A)
        emit userSaysGo();
    else if (button == Qt::Key_D)
        emit userSaysNoGo();
}
