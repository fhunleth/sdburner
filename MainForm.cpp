#include "MainForm.h"
#include "ui_MainForm.h"
#include <QKeyEvent>

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A:
        qCritical("A pressed");
        ui->stackedWidget->setCurrentWidget(ui->aboutPage);
        break;
    case Qt::Key_B:
        qCritical("B pressed");
        ui->stackedWidget->setCurrentWidget(ui->programmingPage);
        break;
    case Qt::Key_C:
        qCritical("C pressed");
        ui->stackedWidget->setCurrentWidget(ui->donePage);
        break;
    case Qt::Key_D:
        qCritical("D pressed");
        ui->stackedWidget->setCurrentWidget(ui->errorPage);
        break;
    default:
        break;
    }
}
