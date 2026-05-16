QT       += core gui network sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    deletedialog.cpp \
    filedialog.cpp \
    filelistdialog.cpp \
    fileviewdialog.cpp \
    httpmgr.cpp \
    main.cpp \
    mainwindow.cpp \
    markdowndelegate.cpp \
    my_list_view.cpp \
    sqlmgr.cpp

HEADERS += \
    deletedialog.h \
    filedialog.h \
    filelistdialog.h \
    fileviewdialog.h \
    httpmgr.h \
    mainwindow.h \
    markdowndelegate.h \
    my_list_view.h \
    sqlmgr.h

FORMS += \
    deletedialog.ui \
    filedialog.ui \
    filelistdialog.ui \
    fileviewdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main_qrc.qrc
