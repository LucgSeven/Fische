#include "welcomewindow.h"
#include "ui_welcomewindow.h"

#include <QMouseEvent>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);

    // 窗口背景透明
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    // SplashScreen、无边框、置顶
    setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::mousePressEvent(QMouseEvent *event)
{
    qPoint = event->globalPos();
}

void WelcomeWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(this->x() + (event->globalX() - qPoint.x()),
               this->y() + (event->globalY() - qPoint.y()));
    qPoint = event->globalPos();
}

void WelcomeWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qPoint = event->globalPos();
}

void WelcomeWindow::on_closeBtn_clicked()
{
    this->deleteLater();
}
