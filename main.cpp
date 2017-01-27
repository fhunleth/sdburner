#include <QApplication>

#include "MainForm.h"
#include "UiWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UiWidget w;
    w.show();

    return a.exec();
}
