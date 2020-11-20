#ifndef RESIZEBOOKAREA_H
#define RESIZEBOOKAREA_H

#include <QDialog>

namespace Ui {
class ResizeBookArea;
}

class ResizeBookArea : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeBookArea(QWidget *parent = nullptr);
    ~ResizeBookArea();
    void initSizeData(int w, int h); // 初始化本地保存窗口大小信息

signals:
    void confirmResize(int w, int h); // 点击确认按钮信号

private slots:
    void on_okBtn_clicked(); // 点击确认按钮
    void on_cancelBtn_clicked(); // 点击取消按钮

private:
    Ui::ResizeBookArea *ui;
};

#endif // RESIZEBOOKAREA_H
