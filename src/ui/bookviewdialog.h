#ifndef BOOKVIEWDIALOG_H
#define BOOKVIEWDIALOG_H

#include "../pageinfo.h"
#include "../fischecofig.h"

#include <QDialog>
#include <QAbstractNativeEventFilter>
#include <QTimer>
#include <Windows.h>

namespace Ui {
class BookViewDialog;
}

class BookViewDialog : public QDialog,QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit BookViewDialog(QWidget *parent = nullptr, PageInfo *pageInfo = nullptr);
    ~BookViewDialog();
    PageInfo *pageInfo;
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *);
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

private:
    Ui::BookViewDialog *ui;
    QString getCorrectUnicode(const QByteArray &ba); // 文本乱码处理
    QTimer *timer; // 定时器
    QPoint qPoint; // 鼠标指针对象
    FischeCofig *config; // 本地配置信息

    bool autoRead = false; // 自动阅读标识
    QTimer *autoTimer; // 自动阅读定时器
    HWND HWND_WINID; // 窗口句柄
    ATOM FISCHE_KEY_PREVIOUS; // 热键标识：前进
    ATOM FISCHE_KEY_NEXT; // 热键标识：后退
    ATOM FISCHE_KEY_HIDE; // 热键标识：隐藏
    ATOM FISCHE_KEY_RESHOW; // 热键标识：展示
    ATOM FISCHE_KEY_AUTO_START; // 热键标识:自动阅读开始
    ATOM FISCHE_KEY_AUTO_END; // 热键标识：自动阅读结束

    void setFontColor(QColor color); // 设置字体颜色
    void registerHotKey(); // 热键注册
    void unregisterHotKey(); // 取消热键注册
    void registerReadHotKey(); // 阅读类热键注册
    void unregisterReadHotKey(); // 取消阅读类热键注册
    void registerDoHotKey(); // 操作类热键注册
    void unregisterDoHotKey(); // 取消操作类热键注册

signals:
    void bookClosed(); // 退出阅读信号

private slots:
    void positionSave(); // 阅读进度保存槽
    void initPosition(); // 初始化阅读进度槽
    void afterLoad(); // 页面加载完成后执行
    void onHide(); // 窗口隐藏时执行
    void onClose(); // 窗口关闭时执行
    void openResize(); // 打开resize窗口
    void onResize(int w, int h); // 修改窗口大小槽
    void onRecolor(); // 修改文字颜色槽
    void recolor1(); // 修改内置文字颜色1
    void recolor2(); // 修改内置文字颜色2
    void recolor3(); // 修改内置文字颜色3
    void recolor4(); // 修改内置文字颜色4
    void recolor5(); // 修改内置文字颜色5
    void backwardLines(); // 后退N行
    void forwardLines(); // 前进N行

};

#endif // BOOKVIEWDIALOG_H
