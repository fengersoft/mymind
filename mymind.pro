QT       += core gui sql

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
    src/aboutdialog.cpp \
    src/api/capture/capturewindow.cpp \
    src/api/color/colortable.cpp \
    src/api/controls/colorwidget.cpp \
    src/api/controls/pixmapviewer.cpp \
    src/api/controls/showpixmapdialog.cpp \
    src/api/file/fileseacher.cpp \
    src/api/sql/sqldef.cpp \
    src/api/sql/sqlitewrapper.cpp \
    src/editcolordialog.cpp \
    src/editlinkdialog.cpp \
    src/editmarknodedialog.cpp \
    src/editnodedialog.cpp \
    src/editremarkdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mindmapobject.cpp \
    src/mindmapwidget.cpp \
    src/mindmapwidgettoolbar.cpp \
    src/mydao.cpp \
    src/openprojectdialog.cpp \
    src/openrecentdialog.cpp \
    src/searchandreplacedialog.cpp \
    src/setbackgrounddialog.cpp \
    src/showmindlineoutdialog.cpp

HEADERS += \
    src/aboutdialog.h \
    src/api/capture/capturewindow.h \
    src/api/color/colortable.h \
    src/api/controls/colorwidget.h \
    src/api/controls/pixmapviewer.h \
    src/api/controls/showpixmapdialog.h \
    src/api/file/fileseacher.h \
    src/api/sql/sqldef.h \
    src/api/sql/sqlitewrapper.h \
    src/editcolordialog.h \
    src/editlinkdialog.h \
    src/editmarknodedialog.h \
    src/editnodedialog.h \
    src/editremarkdialog.h \
    src/mainwindow.h \
    src/mindmapobject.h \
    src/mindmapwidget.h \
    src/mindmapwidgettoolbar.h \
    src/mydao.h \
    src/openprojectdialog.h \
    src/openrecentdialog.h \
    src/searchandreplacedialog.h \
    src/setbackgrounddialog.h \
    src/showmindlineoutdialog.h

FORMS += \
    src/aboutdialog.ui \
    src/api/capture/capturewindow.ui \
    src/api/controls/colorwidget.ui \
    src/api/controls/pixmapviewer.ui \
    src/api/controls/showpixmapdialog.ui \
    src/editcolordialog.ui \
    src/editlinkdialog.ui \
    src/editmarknodedialog.ui \
    src/editnodedialog.ui \
    src/editremarkdialog.ui \
    src/mainwindow.ui \
    src/mindmapwidget.ui \
    src/mindmapwidgettoolbar.ui \
    src/openprojectdialog.ui \
    src/openrecentdialog.ui \
    src/searchandreplacedialog.ui \
    src/setbackgrounddialog.ui \
    src/showmindlineoutdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DESTDIR=$$PWD/bin
RC_ICONS=app.ico

RESOURCES += \
    app.qrc

DISTFILES += \
    android_resourcecs/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR=$$PWD/android_resourcecs

