#include "bookinfo.h"
#include "ui_bookinfo.h"

BookInfo::BookInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookInfo)
{
    ui->setupUi(this);
}

BookInfo::~BookInfo()
{
    delete ui;
}

void BookInfo::setInfo(PageInfo *pageInfo)
{
    ui->bookNameLabel->setText(pageInfo->bookName);
    ui->readLevel->setText(QString::number(pageInfo->readLevel * 100, 'f', 2) + "%");
    this->pageInfo = pageInfo;
}


void BookInfo::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) // 左键点击
    {
        emit this->bookInfoLeftClicked(pageInfo->index, pageInfo->bookName, pageInfo->bookPath, pageInfo->readLevel);
    }
    if (event->button() == Qt::RightButton) // 右键点击
    {
        emit this->bookInfoRightClicked(pageInfo->index, pageInfo->bookName);
    }

}


