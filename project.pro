QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    funlist.cpp \
    img_reader.cpp \
    main.cpp \
    my_close.cpp \
    my_wi.cpp \
    mycamera.cpp \
    widget.cpp

HEADERS += \
    funlist.h \
    img_reader.h \
    my_close.h \
    my_wi.h \
    mycamera.h \
    widget.h

FORMS += \
    funlist.ui \
    mycamera.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += D:/Libs/VLC-Qt_1.1.0_win64_msvc2015/lib/VLC*
else:win32:CONFIG(debug, debug|release): LIBS += D:/Libs/VLC-Qt_1.1.0_win64_msvc2015/libd/VLC*

INCLUDEPATH += D:/Libs/VLC-Qt_1.1.0_win64_msvc2015/include
DEPENDPATH += D:/Libs/VLC-Qt_1.1.0_win64_msvc2015/include

#win32: LIBS += $$PWD/../../Libs/VLC-2.2.4/lib/lib*.lib

#INCLUDEPATH += $$PWD/../../Libs/VLC-2.2.4/include
#DEPENDPATH += $$PWD/../../Libs/VLC-2.2.4/include
