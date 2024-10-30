QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    funlist.cpp \
    img_reader.cpp \
    main.cpp \
    my_close.cpp \
    my_img.cpp \
    my_transform.cpp \
    my_wi.cpp \
    myvideosurface.cpp \
    widget.cpp

HEADERS += \
    funlist.h \
    img_reader.h \
    my_close.h \
    my_img.h \
    my_transform.h \
    my_wi.h \
    myvideosurface.h \
    widget.h

FORMS += \
    funlist.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32: LIBS += $$PWD/Libs/opencv_4.2/lib/libopencv_*.dll.a

INCLUDEPATH += $$PWD/Libs/opencv_4.2/include
DEPENDPATH += $$PWD/Libs/opencv_4.2/include
