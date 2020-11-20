#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookinfo.h"
#include "../pageinfo.h"
#include "bookviewdialog.h"
#include "welcomewindow.h"

#include <QList>
#include <QFileDialog>
#include <QAction>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 窗口背景透明
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    // 无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);

    setFixedSize(this->width(),this->height());

    // 系统托盘图标
    systemTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/images/fische.png");
    systemTrayIcon->setIcon(icon);
    systemTrayIcon->setToolTip("Fische摸鱼小程序");
    systemTrayIcon->show();
    systemTrayIcon->showMessage("", "屏幕右下角可以找我哟~", icon, 5000);

    // 读取本地配置
    config = new FischeCofig();
    this->initBookList(config);

    connect(ui->addBtn, SIGNAL(clicked(bool)), this, SLOT(addBook()));
    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qPoint = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(this->x() + (event->globalX() - qPoint.x()),
               this->y() + (event->globalY() - qPoint.y()));
    qPoint = event->globalPos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qPoint = event->globalPos();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reShow()
{
    config = new FischeCofig();
    this->initBookList(config);
    this->show();
}

void MainWindow::onActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    QMenu *menu = new QMenu(this);
    QAction *backAction = new QAction("回到主页", this);
    menu->addAction(backAction);
    QAction *aboutAction = new QAction("软件说明", this);
    menu->addAction(aboutAction);
    QAction *closeAction = new QAction("退出", this);
    menu->addAction(closeAction);
    connect(backAction, SIGNAL(triggered()), this, SLOT(wakeUp()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    QPoint qPoint = cursor().pos();
    switch(reason){

    case QSystemTrayIcon::Trigger: // 单击托盘图标
        break;

    case QSystemTrayIcon::DoubleClick: // 双击托盘图标
        this->wakeUp();
        break;
    case QSystemTrayIcon::Context: // 右键托盘图标
        menu->move(qPoint.x(), qPoint.y()-80);
        menu->show();
        break;
    default:
        break;
    }
}

void MainWindow:: addBook()
{
    const QList<QUrl> fileUrls = QFileDialog::getOpenFileUrls(this, "添加书籍", QUrl(), "TXT文本(*.txt)");
    for (int i = 0; i < fileUrls.size(); i++) {
        QUrl fileUrl = fileUrls.at(i);

        QString fileName = fileUrl.fileName();
        if (fileName.contains(".")) {
            fileName = fileName.mid(0, fileName.lastIndexOf("."));
        }
        QString filePath = fileUrl.path();
        if (filePath.startsWith("/") || filePath.startsWith("\\")) {
            filePath = filePath.remove(0,1);
        }

        PageInfo *pageInfo = new PageInfo(0, fileName, filePath, 0);
        this->config->addBook(*pageInfo);
    }
    this->initBookList(config);
}

void MainWindow:: deleteBook()
{
    int index = this->bookIndex;
    this->config->deleteBook(index);
    this->initBookList(config);
}

void MainWindow::wakeUp()
{
    this->show();

    // 设置窗口活跃
    this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

    emit this->onWindowReshow();
}

void MainWindow::about()
{
    WelcomeWindow *welcome = new WelcomeWindow();
    welcome->show();
}

void MainWindow:: clickedLeftBook(int index, QString bookName, QString bookPath, double readLevel)
{
    this->hide();
    PageInfo *pageInfo = new PageInfo(index, bookName, bookPath, readLevel);
    BookViewDialog *bookviewdialog = new BookViewDialog(this, pageInfo);
    bookviewdialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    bookviewdialog->setParent(this);
    bookviewdialog->show();

    connect(bookviewdialog, SIGNAL(bookClosed()), this, SLOT(reShow()));
    connect(this, SIGNAL(onWindowReshow()), bookviewdialog, SLOT(onClose()));
}

void MainWindow:: clickedRightBook(int index, QString bookName)
{
    this->bookIndex = index;

    QMenu *menu = new QMenu(this);

    QAction *cancelAction = new QAction("取消", this);
    menu->addAction(cancelAction);

    menu->addSeparator();

    QAction *deleteAction = new QAction("删除【" + bookName + "】", this);
    menu->addAction(deleteAction);

    menu->move(cursor().pos());
    menu->show();

    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteBook()));
    connect(cancelAction, SIGNAL(triggered()), cancelAction, SLOT(deleteLater()));
}

void MainWindow::initBookList(FischeCofig *config)
{
    ui->bookListWidget->clear();
    QList<PageInfo> bookList = config->bookList;
    for (int i = 0; i < bookList.size(); i++) {
        PageInfo pageInfo = bookList.at(i);

        BookInfo *bookinfo = new BookInfo();
        PageInfo *newPage = new PageInfo(pageInfo.index, pageInfo.bookName, pageInfo.bookPath, pageInfo.readLevel);
        bookinfo->setInfo(newPage);
        QListWidgetItem *item = new QListWidgetItem();
        ui->bookListWidget->addItem(item);
        ui->bookListWidget->setItemWidget(item, bookinfo);

        connect(bookinfo, SIGNAL(bookInfoLeftClicked(int, QString, QString, double)), this, SLOT(clickedLeftBook(int, QString, QString, double)));
        connect(bookinfo, SIGNAL(bookInfoRightClicked(int, QString)), this, SLOT(clickedRightBook(int, QString)));
    }
}

void MainWindow::on_minBtn_clicked()
{
    this->hide();
}

void MainWindow::on_closeBtn_clicked()
{
    this->close();
}
