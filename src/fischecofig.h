#ifndef FISCHECOFIG_H
#define FISCHECOFIG_H

#include "pageinfo.h"
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QList>

/**
 * @brief version 当前版本号
 */
static QString version = "0.1";

/**
 * @brief orgName organization
 */
static QString orgName = "Fische";

static QString groupGenaral = "genaral";
static QString nameVersion = "version";
static QString groupBaseInfo = "baseInfo";
static QString nameBookNum = "bookNum";
static QString nameBookList = "bookList";
static QString nameBookName = "bookName";
static QString nameBookPath = "bookPath";
static QString nameReadLevel = "readLevel";
static QString groupWindow = "window";
static QString nameWindowHeight = "windowHeight";
static QString nameWindowWidth = "windowWidth";
static QString nameFontColor = "fontColor";

class FischeCofig
{
public:
    FischeCofig();
    int bookNum; // 书架上书籍总数
    QList<PageInfo> bookList; // 书籍信息列表
    int windowHeight; // 窗口高度
    int windowWidth; // 窗口宽度
    QString fontColor; // 字体颜色

    void addBook(PageInfo pageInfo); //增加书籍
    void deleteBook(int index); // 删除书籍

    static void updateReadLevel(int index, double readLevel); // 更新书本阅读进度
    static void updateWindowSetting(int w, int h); // 更新本地窗口设置
    static void updateFontColor(QString color); // 更新本地缓存字体颜色

    static QString currentVersion(); // 获取最新版本号
    static QString lastVersion(); // 获取本地保存的版本号
    static void updateVersion(); // 更新本地版本号至最新

private:
    QSettings settings;
    void readConfig(); // 读取本地配置信息
};

#endif // FISCHECOFIG_H
