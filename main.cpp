#include <QApplication>

#include "MainForm.h"
#include "UiWidget.h"
#include "FwupWrapper.h"
#include "App.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FwupWrapper fwup;

    qDebug("Version=%s", qPrintable(fwup.version()));

    QList<FwupWrapper::MemoryCard> cards = fwup.detectMemoryCards();
    foreach (FwupWrapper::MemoryCard card, cards) {
        qDebug("Detected: %s", qPrintable(card.path));
    }

    App app;
    app.start();

#if 0
    UiWidget w;
    w.show();
#endif

    return a.exec();
}
