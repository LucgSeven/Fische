#include "ui/mainwindow.h"
#include "ui/welcomewindow.h"
#include "fischecofig.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载qss样式
    QFile qss(":style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    MainWindow w;
    w.show();

    // 若版本号更新，显示欢迎页
    if (FischeCofig::currentVersion() != FischeCofig::lastVersion()) {
        FischeCofig::updateVersion();

        WelcomeWindow *welcome = new WelcomeWindow();
        welcome->show();
    }

    return a.exec();
}
