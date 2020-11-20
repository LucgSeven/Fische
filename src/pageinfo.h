#ifndef PAGEINFO_H
#define PAGEINFO_H

#include <QString>

class PageInfo
{
public:
    PageInfo();
    PageInfo(int index, QString bookName, QString bookPath, double readLevel);
    int index; // 书籍序号
    QString bookName; // 书籍名称
    QString bookPath; // 书籍路径
    double readLevel; // 阅读进度
};

#endif // PAGEINFO_H
