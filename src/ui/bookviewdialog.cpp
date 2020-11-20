#include "bookviewdialog.h"
#include "ui_bookviewdialog.h"
#include "../fischecofig.h"
#include "resizebookarea.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QString>
#include <QScrollBar>
#include <QtMath>
#include <QMenu>
#include <QAction>
#include <QColorDialog>
#include <QColor>
#include <QPalette>

BookViewDialog::BookViewDialog(QWidget *parent, PageInfo *pageInfo) :
    QDialog(parent),
    ui(new Ui::BookViewDialog)
{

    ui->setupUi(this);

    // 获取windows窗口句柄（winId()与下一行WA_TranslucentBackground冲突导致运行报错）
    HWND_WINID = HWND(this->internalWinId());

    // 窗口背景透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    // 读取本地配置信息，初始化窗口大小、字体颜色
    config = new FischeCofig();
    this->resize(config->windowWidth, config->windowHeight);
    ui->bookText->setTextColor(QColor(config->fontColor));

    this->pageInfo = pageInfo;

    // 只读打开文件
    QFile file(pageInfo->bookPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Fische"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(pageInfo->bookPath), file.errorString()));
        return;
    }

    QByteArray text = file.readAll();
    QString content = this->getCorrectUnicode(text); // 文本乱码处理
    ui->bookText->setText(content);

    // 文本框注册事件过滤器，获取该组件加载完成事件
    ui->bookText->installEventFilter(this);

    // 每隔30自动保存阅读进度
    timer = new QTimer(this);
    timer->stop();
    timer->setInterval(30000);
    connect(timer, SIGNAL(timeout()), this, SLOT(positionSave()));

    // 自动阅读定时器
    autoTimer = new QTimer(this);
    autoTimer->stop();
    autoTimer->setInterval(3000);
    autoRead = false;
    connect(autoTimer, SIGNAL(timeout()), this, SLOT(forwardLines()));

    // 全局热键
    this->registerHotKey();
    qApp->installNativeEventFilter(this);
}

bool BookViewDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MetaCall && obj == ui->bookText) {
        // 文本框组件加载完成
        emit afterLoad();
    }
    return QDialog::eventFilter(obj, event);
}

void BookViewDialog::closeEvent(QCloseEvent *)
{
    // 窗口关闭时停止定时器
    timer->stop();
    // 保存当前阅读进度
    emit this->positionSave();
}

void BookViewDialog::mousePressEvent(QMouseEvent *event)
{
    qPoint = event->globalPos();
}

void BookViewDialog::mouseMoveEvent(QMouseEvent *event)
{
    this->move(this->x() + (event->globalX() - qPoint.x()),
               this->y() + (event->globalY() - qPoint.y()));
    qPoint = event->globalPos();
}

void BookViewDialog::mouseReleaseEvent(QMouseEvent *event)
{
    qPoint = event->globalPos();
}

void BookViewDialog::contextMenuEvent(QContextMenuEvent *) // 右键菜单设置
{
    QMenu *menu = new QMenu(this);

    QAction *hideAction = new QAction("隐藏", this);
    menu->addAction(hideAction);

    QAction *closeAction = new QAction("重选书籍", this);
    menu->addAction(closeAction);

    menu->addSeparator();
    QAction *resizeAction = new QAction("修改窗口大小", this);

    menu->addAction(resizeAction);
    QMenu *colorMenu = new QMenu("修改字体颜色", this);
    QAction *recolorAction1 = new QAction("钝色", this); // #727171
    QPixmap pixmap1(100, 100);
    pixmap1.fill(QColor("#ff727171"));
    recolorAction1->setIcon(QIcon(pixmap1));
    QAction *recolorAction2 = new QAction("绾色", this); // #a98175
    QPixmap pixmap2(100, 100);
    pixmap2.fill(QColor("#ffa98175"));
    recolorAction2->setIcon(QIcon(pixmap2));
    QAction *recolorAction3 = new QAction("银白", this); // #e9e7ef
    QPixmap pixmap3(100, 100);
    pixmap3.fill(QColor("#ffe9e7ef"));
    recolorAction3->setIcon(QIcon(pixmap3));
    QAction *recolorAction4 = new QAction("绢鼠", this); // #dddcd6
    QPixmap pixmap4(100, 100);
    pixmap4.fill(QColor("#ffdddcd6"));
    recolorAction4->setIcon(QIcon(pixmap4));
    QAction *recolorAction5 = new QAction("绛紫", this); // #8c4356
    QPixmap pixmap5(100, 100);
    pixmap5.fill(QColor("#ff8c4356"));
    recolorAction5->setIcon(QIcon(pixmap5));
    QAction *otherColor = new QAction("其他...", this);
    colorMenu->addAction(recolorAction1);
    colorMenu->addAction(recolorAction2);
    colorMenu->addAction(recolorAction3);
    colorMenu->addAction(recolorAction4);
    colorMenu->addAction(recolorAction5);
    colorMenu->addAction(otherColor);
    menu->addMenu(colorMenu);

    menu->move(cursor().pos());
    menu->show();

    connect(hideAction, SIGNAL(triggered()), this, SLOT(onHide()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(onClose()));
    connect(resizeAction, SIGNAL(triggered()), this, SLOT(openResize()));
    connect(recolorAction1, SIGNAL(triggered()), this, SLOT(recolor1()));
    connect(recolorAction2, SIGNAL(triggered()), this, SLOT(recolor2()));
    connect(recolorAction3, SIGNAL(triggered()), this, SLOT(recolor3()));
    connect(recolorAction4, SIGNAL(triggered()), this, SLOT(recolor4()));
    connect(recolorAction5, SIGNAL(triggered()), this, SLOT(recolor5()));
    connect(otherColor, SIGNAL(triggered()), this, SLOT(onRecolor()));
}

void BookViewDialog::onHide()
{
    // 取消热键注册
    this->unregisterHotKey();
    // 隐藏窗口
    this->hide();
    // 移除事件过滤器
    ui->bookText->removeEventFilter(this);
    qApp->removeNativeEventFilter(this);
}

void BookViewDialog::onClose()
{
    // 取消热键注册
    this->unregisterHotKey();
    // 关闭窗口
    this->close();
    // 发送关闭信号通知主窗口
    emit this->bookClosed();
    // 移除事件过滤器
    ui->bookText->removeEventFilter(this);
    qApp->removeNativeEventFilter(this);
}

void BookViewDialog::openResize()
{
    int w = this->width();
    int h = this->height();
    ResizeBookArea *resizeBookArea = new ResizeBookArea();
    resizeBookArea->initSizeData(w, h);
    resizeBookArea->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    resizeBookArea->show();

    connect(resizeBookArea, SIGNAL(confirmResize(int, int)), this, SLOT(onResize(int, int)));
}

void BookViewDialog::onRecolor()
{
    QColor c = QColorDialog::getColor(Qt::black, this, tr("选择字体颜色"), QColorDialog::ShowAlphaChannel);
    this->setFontColor(c);
}

void BookViewDialog::recolor1(){
    this->setFontColor(QColor("#ff727171"));
}

void BookViewDialog::recolor2(){
    this->setFontColor(QColor("#ffa98175"));
}

void BookViewDialog::recolor3(){
    this->setFontColor(QColor("#ffe9e7ef"));
}

void BookViewDialog::recolor4(){
    this->setFontColor(QColor("#ffdddcd6"));
}

void BookViewDialog::recolor5(){
    this->setFontColor(QColor("#ff8c4356"));
}

void BookViewDialog::onResize(int w, int h)
{
    timer->stop();

    this->positionSave();
    this->resize(w, h);
    this->initPosition();

    timer->start();
}

void BookViewDialog::afterLoad(){
    if (0 == ui->bookText->verticalScrollBar()->maximum()) { // 不存在滚动条
        return;
    }
    emit this->initPosition();
    timer->start();
}

void BookViewDialog::initPosition()
{
    QScrollBar *scrollBar = ui->bookText->verticalScrollBar();
    int maximum = scrollBar->maximum();

    // 滚动条最大值*阅读进度百分比取整，设置滚动条位置值
    int value = qFloor(maximum * pageInfo->readLevel);
    scrollBar->setValue(value);
}

void BookViewDialog::positionSave()
{
    QScrollBar *scrollBar = ui->bookText->verticalScrollBar();

    int value = scrollBar->value(); // 滚动条当前值
    int maximum = scrollBar->maximum(); // 滚动条最大值
    double readPosion;
    if (0 == maximum) {
        readPosion = 1.00;
    } else {
        readPosion = QString::number((value  * 1.0000000000/maximum),'f', 10).toDouble();
    }

    this->pageInfo->readLevel = readPosion;
    // 保存本地阅读进度
    FischeCofig::updateReadLevel(this->pageInfo->index, readPosion);
}

QString BookViewDialog::getCorrectUnicode(const QByteArray &ba)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode( ba.constData(), 1, &state);

    if (state.invalidChars) {
        text = QTextCodec::codecForName( "GBK" )->toUnicode(ba);
    } else {
        text = ba;
    }

    return text;
}

void BookViewDialog::setFontColor(QColor color)
{
    if (!color.isValid()) {
        return;
    }
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    ui->bookText->setTextColor(color);
    ui->bookText->setText(ui->bookText->toPlainText());
    QGuiApplication::restoreOverrideCursor();
    FischeCofig::updateFontColor(color.name(QColor::HexArgb));
}

void BookViewDialog::backwardLines()
{
    QScrollBar *scrollBar = ui->bookText->verticalScrollBar();
    int value = scrollBar->value();
    if (value > 30) {
        ui->bookText->verticalScrollBar()->setValue(value - 30);
    }
}

void BookViewDialog::forwardLines()
{
    QScrollBar *scrollBar = ui->bookText->verticalScrollBar();
    int value = scrollBar->value();
    int maximum = scrollBar->maximum();
    if (value < maximum) {
        ui->bookText->verticalScrollBar()->setValue(value + 30);
    }
}

bool BookViewDialog::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    if (eventType == "windows_generic_MSG") {
        MSG* msg = reinterpret_cast<MSG*>(message);
        if(msg->message == WM_HOTKEY) {
            if (msg->wParam == FISCHE_KEY_PREVIOUS) { // 后退
                this->backwardLines();

            } else if (msg->wParam == FISCHE_KEY_NEXT){ // 前进
                this->forwardLines();

            } else if (msg->wParam == FISCHE_KEY_HIDE){ // 隐藏
                this->hide();
                // 取消阅读类热键注册
                this->unregisterReadHotKey();
                if (autoRead) {
                    autoRead = false;
                    autoTimer->stop();
                }

            } else if (msg->wParam == FISCHE_KEY_RESHOW){ // 展示
                this->show();
                // 阅读类热键注册
                this->registerReadHotKey();

            } else if (msg->wParam == FISCHE_KEY_AUTO_START){ // 自动阅读开始
                if (!autoRead) {
                    autoRead = true;
                    autoTimer->start();
                }
            } else if (msg->wParam == FISCHE_KEY_AUTO_END){ // 自动阅读结束
                if (autoRead) {
                    autoRead = false;
                    autoTimer->stop();
                }
            }
        }
    }
    return QWidget::nativeEvent(eventType,message,result);
}

void BookViewDialog::registerHotKey()
{
    // 阅读类热键注册
    this->registerReadHotKey();
    // 操作类热键注册
    this->registerDoHotKey();
}

void BookViewDialog::unregisterHotKey()
{
    // 取消阅读类热键注册
    this->unregisterReadHotKey();
    // 取消操作类热键注册
    this->unregisterDoHotKey();
}

void BookViewDialog::registerReadHotKey() // 阅读类热键注册
{
    // 获取热键标识
    FISCHE_KEY_PREVIOUS = GlobalAddAtomA("FISCHE_KEY_PREVIOUS");
    FISCHE_KEY_NEXT = GlobalAddAtomA("FISCHE_KEY_NEXT");
    FISCHE_KEY_AUTO_START = GlobalAddAtomA("FISCHE_KEY_AUTO_START");
    FISCHE_KEY_AUTO_END = GlobalAddAtomA("FISCHE_KEY_AUTO_END");

    // 注册全局热键
    RegisterHotKey(HWND_WINID, FISCHE_KEY_PREVIOUS, MOD_ALT, VK_UP); // 后退 alt+↑
    RegisterHotKey(HWND_WINID, FISCHE_KEY_NEXT, MOD_ALT, VK_DOWN); // 前进 alt+↓
    RegisterHotKey(HWND_WINID, FISCHE_KEY_AUTO_START, MOD_CONTROL + MOD_ALT, VK_NUMPAD0); // 自动阅读开始 ctrl+alt+num0
    RegisterHotKey(HWND_WINID, FISCHE_KEY_AUTO_END, MOD_CONTROL + MOD_ALT, VK_NUMPAD1); // 自动阅读结束 ctrl+alt+num1
}

void BookViewDialog::unregisterReadHotKey() // 取消阅读类热键注册
{
    // 销毁全局热键
    UnregisterHotKey(HWND_WINID, FISCHE_KEY_PREVIOUS);
    UnregisterHotKey(HWND_WINID, FISCHE_KEY_NEXT);
    UnregisterHotKey(HWND_WINID, FISCHE_KEY_AUTO_START);
    UnregisterHotKey(HWND_WINID, FISCHE_KEY_AUTO_END);

    // 释放热键标识
    GlobalDeleteAtom(FISCHE_KEY_PREVIOUS);
    GlobalDeleteAtom(FISCHE_KEY_NEXT);
    GlobalDeleteAtom(FISCHE_KEY_AUTO_START);
    GlobalDeleteAtom(FISCHE_KEY_AUTO_END);

    autoRead = false;
    autoTimer->stop();
}

void BookViewDialog::registerDoHotKey() // 操作类热键注册
{
    // 获取热键标识
    FISCHE_KEY_HIDE = GlobalAddAtomA("FISCHE_KEY_HIDE");
    FISCHE_KEY_RESHOW = GlobalAddAtomA("FISCHE_KEY_RESHOW");

    // 注册全局热键
    RegisterHotKey(HWND_WINID, FISCHE_KEY_HIDE, MOD_ALT, VK_NUMPAD0); // 隐藏 alt+num0
    RegisterHotKey(HWND_WINID, FISCHE_KEY_RESHOW, MOD_ALT, VK_NUMPAD1); // 展示 alt+num1
}

void BookViewDialog::unregisterDoHotKey() // 取消操作类热键注册
{
    // 销毁全局热键
    UnregisterHotKey(HWND_WINID, FISCHE_KEY_HIDE);
    UnregisterHotKey(HWND_WINID, FISCHE_KEY_RESHOW);

    // 释放热键标识
    GlobalDeleteAtom(FISCHE_KEY_HIDE);
    GlobalDeleteAtom(FISCHE_KEY_RESHOW);
}

BookViewDialog::~BookViewDialog()
{
    delete ui;

    this->unregisterHotKey();
    ui->bookText->removeEventFilter(this);
    qApp->removeNativeEventFilter(this);

}
