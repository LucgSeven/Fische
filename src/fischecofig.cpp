#include "fischecofig.h"

#include <QCoreApplication>

FischeCofig::FischeCofig()
{
    readConfig();
}

void FischeCofig::addBook(PageInfo pageInfo)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());

    settings.beginGroup(groupBaseInfo);
    settings.setValue(nameBookNum, bookNum + 1);

    settings.beginReadArray(nameBookList);
    settings.setArrayIndex(bookNum);
    settings.setValue(nameBookName, pageInfo.bookName);
    settings.setValue(nameBookPath, pageInfo.bookPath);
    settings.setValue(nameReadLevel, pageInfo.readLevel);
    settings.endArray();

    settings.endGroup();

    this->readConfig();
}

void FischeCofig::deleteBook(int index)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());

    settings.beginGroup(groupBaseInfo);
    settings.setValue(nameBookNum, bookNum - 1);

    settings.beginReadArray(nameBookList);

    for (int i = index; i < bookNum - 1; i++) {
        settings.setArrayIndex(i + 1);
        QString name = settings.value(nameBookName).toString();
        QString path = settings.value(nameBookPath).toString();
        double level = settings.value(nameReadLevel).toDouble();

        settings.setArrayIndex(i);
        settings.setValue(nameBookName, name);
        settings.setValue(nameBookPath, path);
        settings.setValue(nameReadLevel, level);
    }

    settings.setArrayIndex(bookNum - 1);
    settings.remove(nameBookName);
    settings.remove(nameBookPath);
    settings.remove(nameReadLevel);

    settings.endArray();

    settings.endGroup();

    this->readConfig();
}

void FischeCofig::updateReadLevel(int index, double readLevel)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());
    settings.beginGroup(groupBaseInfo);

    settings.beginReadArray(nameBookList);
    settings.setArrayIndex(index);
    settings.setValue(nameReadLevel, readLevel);
    settings.endArray();

    settings.endGroup();
}

void FischeCofig::updateWindowSetting(int w, int h)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());
    settings.beginGroup(groupWindow);

    settings.setValue(nameWindowHeight, h);
    settings.setValue(nameWindowWidth, w);

    settings.endGroup();
}

void FischeCofig::updateFontColor(QString color)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());
    settings.beginGroup(groupWindow);

    settings.setValue(nameFontColor, color);

    settings.endGroup();
}

QString FischeCofig::currentVersion()
{
    return version;
}

QString FischeCofig::lastVersion()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());
    settings.beginGroup(groupGenaral);
    QString lastVersion = settings.value(nameVersion).toString();
    settings.endGroup();
    return lastVersion;
}

void FischeCofig::updateVersion()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());
    settings.beginGroup(groupGenaral);
    settings.setValue(nameVersion, version);
    settings.endGroup();
}

void FischeCofig::readConfig()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, orgName, QCoreApplication::applicationName());

    settings.beginGroup(groupWindow);
    QString windowSetting = settings.value(nameWindowHeight).toString();
    if (windowSetting.isEmpty()) {
        settings.setValue(nameWindowHeight, 600);
        settings.setValue(nameWindowWidth, 400);
        settings.setValue(nameFontColor, "#ff000000");
    }
    this->windowHeight = settings.value(nameWindowHeight).toInt();
    this->windowWidth = settings.value(nameWindowWidth).toInt();
    this->fontColor = settings.value(nameFontColor).toString();
    settings.endGroup();

    this->bookList.clear();
    settings.beginGroup(groupBaseInfo);
    QString bookNum = settings.value(nameBookNum).toString();
    if (bookNum.isEmpty()) {
        settings.setValue(nameBookNum, 0);
        this->bookNum = 0;
    } else {
        int num = settings.value(nameBookNum).toInt();
        this->bookNum = num;
        settings.beginReadArray(nameBookList);
        for (int i = 0; i < num; i++) {
            settings.setArrayIndex(i);
            PageInfo *pageInfo = new PageInfo(i, settings.value(nameBookName).toString(), settings.value(nameBookPath).toString(), settings.value(nameReadLevel).toDouble());
            this->bookList.append(*pageInfo);
        }
        settings.endArray();
    }
    settings.endGroup();
}
