#include "UiWidget.h"
#include <QPainter>
#include <QKeyEvent>

UiWidget::UiWidget(QWidget *parent) : QWidget(parent)
{
    resize(320, 240);

    currentScreen_ = 0;
    screens_
             << QImage(":/ui/memcard.svg.png")
             << QImage(":/ui/about.svg.png")
             << QImage(":/ui/fun.svg.png")
             << QImage(":/ui/fun1_5.svg.png")
             << QImage(":/ui/fun2.svg.png")
             << QImage(":/ui/programornot.svg.png")
             << QImage(":/ui/programming.svg.png")
             << QImage(":/ui/finished.svg.png")
             << QImage(":/ui/remvsdcard.svg.png");
}

void UiWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A:
        qCritical("A pressed");
        currentScreen_++;
        if (currentScreen_ == screens_.count())
            currentScreen_ = 0;
        break;
    case Qt::Key_B:
        qCritical("B pressed");
        currentScreen_--;
        if (currentScreen_ < 0)
            currentScreen_ = screens_.count() - 1;
        break;
    case Qt::Key_C:
        qCritical("C pressed");
        break;
    case Qt::Key_D:
        qCritical("D pressed");
        break;
    default:
        break;
    }

    update();
}

void UiWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.drawImage(0, 0, screens_.at(currentScreen_));
}
