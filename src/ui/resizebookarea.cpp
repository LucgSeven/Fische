#include "resizebookarea.h"
#include "ui_resizebookarea.h"
#include "../fischecofig.h"

#include <QMessageBox>
#include <QIntValidator>

ResizeBookArea::ResizeBookArea(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeBookArea)
{
    ui->setupUi(this);

    // 限制输入数字范围
    ui->widthEdit->setValidator(new QIntValidator(0, 99999, this));
    ui->heightEdit->setValidator(new QIntValidator(0, 99999, this));
}

void ResizeBookArea::initSizeData(int w, int h)
{
    this->ui->widthEdit->setText(QString::number(w));
    this->ui->heightEdit->setText(QString::number(h));
}

ResizeBookArea::~ResizeBookArea()
{
    delete ui;
}

void ResizeBookArea::on_okBtn_clicked()
{
    QString widthText = ui->widthEdit->text();
    QString heightText = ui->heightEdit->text();

    if (widthText.isEmpty() || heightText.isEmpty() || widthText.isNull() || heightText.isNull()) {
        QMessageBox::warning(this, tr("Fische"), tr("请输入正确的宽度和高度！"));
        return;
    }

    int w = widthText.toInt();
    int h = heightText.toInt();

    emit this->confirmResize(w, h);

    FischeCofig::updateWindowSetting(w, h);

    this->deleteLater();
}

void ResizeBookArea::on_cancelBtn_clicked()
{
    this->deleteLater();
}
