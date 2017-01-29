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

    enum Screen {
        INSERT_SDCARD = 0,
        ASK_USER,
        PROGRAMMING,
        PROGRAMMING_ERROR,
        PROGRAMMING_SUCCESS,
        ABOUT,
        FUN1,
        FUN2,
        FUN3,

        NUM_SCREENS
    };

    void showScreen(Screen screen);

    void setProgress(int progress);

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void userSaysGo();
    void userSaysNoGo();

public slots:

private:
    typedef void (UiWidget::*HandleButtonFn)(int button);
    struct ScreenInfo {
        const char *imageFile;
        bool showProgress;
        HandleButtonFn handleButton;
    };

private:
    void handleInsertSdcardButton(int button);
    void handleFunScreenButton(int button);
    void handleAboutScreenButton(int button);
    void handleAskUserButton(int button);

    static const ScreenInfo screens_[NUM_SCREENS];
    QImage screenImages_[NUM_SCREENS];

    Screen currentScreen_;
    int progress_;
};

#endif // UIWIDGET_H
