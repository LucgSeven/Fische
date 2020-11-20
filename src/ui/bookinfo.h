#ifndef BOOKINFO_H
#define BOOKINFO_H

#include "../pageinfo.h"
#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class BookInfo;
}

class BookInfo : public QWidget
{
    Q_OBJECT

public:
    explicit BookInfo(QWidget *parent = nullptr);
    ~BookInfo();
    void setInfo(PageInfo *pageInfo); // 初始化书籍信息
    PageInfo *pageInfo; // 书籍信息
    void mousePressEvent(QMouseEvent *event);

signals:
    void bookInfoLeftClicked(int index, QString bookName, QString bookPath, double readLevel); // 鼠标左键单击信号
    void bookInfoRightClicked(int index, QString bookName); // 鼠标右键单击信号

private:
    Ui::BookInfo *ui;
};

#endif // BOOKINFO_H
