#ifndef UIWIDGET_H
#define UIWIDGET_H

#include <QWidget>
#include <QImage>
#include <QList>

class UiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UiWidget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    QList<QImage> screens_;
    int currentScreen_;
};

#endif // UIWIDGET_H
