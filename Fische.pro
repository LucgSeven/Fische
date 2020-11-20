QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/fischecofig.cpp \
    src/main.cpp \
    src/pageinfo.cpp \
    src/ui/bookinfo.cpp \
    src/ui/bookviewdialog.cpp \
    src/ui/mainwindow.cpp \
    src/ui/resizebookarea.cpp \
    src/ui/welcomewindow.cpp

HEADERS += \
    src/fischecofig.h \
    src/pageinfo.h \
    src/ui/bookinfo.h \
    src/ui/bookviewdialog.h \
    src/ui/mainwindow.h \
    src/ui/resizebookarea.h \
    src/ui/welcomewindow.h

FORMS += \
    src/ui/bookinfo.ui \
    src/ui/bookviewdialog.ui \
    src/ui/mainwindow.ui \
    src/ui/resizebookarea.ui \
    src/ui/welcomewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    fische.qrc

DISTFILES += \
    fische.rc \
    style.qss

RC_FILE += \
    fische.rc

LIBS += User32.Lib

msvc{
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
