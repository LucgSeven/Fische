#include "pageinfo.h"

PageInfo::PageInfo()
{

}

PageInfo::PageInfo(int index, QString bookName, QString bookPath, double readLevel)
{
    this->index = index;
    this->bookName = bookName;
    this->bookPath = bookPath;
    this->readLevel = readLevel;
}
