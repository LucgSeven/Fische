#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "../fischecofig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_minBtn_clicked(); // 点击最小化按钮
    void on_closeBtn_clicked(); // 点击关闭按钮
    void addBook(); // 增加书籍
    void clickedLeftBook(int index, QString bookName, QString bookPath, double readLevel); // 左键书籍
    void clickedRightBook(int index, QString bookName); // 右键书籍
    void onActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason); // 系统托盘点击事件
    void reShow(); // 窗口重现
    void deleteBook(); // 删除书籍
    void wakeUp(); // 窗口唤醒
    void about(); // 软件介绍页

signals:
    void onWindowReshow(); // 窗口重现信号

private:
    Ui::MainWindow *ui;
    QPoint qPoint; // 鼠标指针对象
    FischeCofig *config; // 本地配置信息
    void initBookList(FischeCofig *config); // 根据本地配置信息初始化书架列表
    QSystemTrayIcon *systemTrayIcon; // 系统托盘对象
    int bookIndex; // 当前点击书籍序号

};
#endif // MAINWINDOW_H
