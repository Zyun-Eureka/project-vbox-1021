QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ModelParser.cpp \
    ModelProcessor.cpp \
    Yolov5Session.cpp \
    cutboxwin.cpp \
    funlist.cpp \
    img_reader.cpp \
    main.cpp \
    my_close.cpp \
    my_img.cpp \
    my_threadpool.cpp \
    my_transform.cpp \
    my_wi.cpp \
    myvideosurface.cpp \
    widget.cpp

HEADERS += \
    ISession.h \
    Mics.h \
    Model.h \
    ModelParser.h \
    ModelProcessor.h \
    YoloDefine.h \
    Yolov5Session.h \
    cutboxwin.h \
    funlist.h \
    img_reader.h \
    my_close.h \
    my_img.h \
    my_threadpool.h \
    my_transform.h \
    my_wi.h \
    myvideosurface.h \
    widget.h

FORMS += \
    cutboxwin.ui \
    funlist.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#encoding
QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8

#opencv
win32: LIBS += $$PWD/Libs/opencv_4.2/lib/opencv_*
win32: INCLUDEPATH += $$PWD/Libs/opencv_4.2/include/
win32: DEPENDPATH += $$PWD/Libs/opencv_4.2/include/

unix:!macx:LIBS += -L$$PWD/../../../CMAKEINSTALL/opencv/lib/ -lopencv*
unix:!macx:INCLUDEPATH += $$PWD/../../../CMAKEINSTALL/opencv/include/opencv4/
unix:!macx:DEPENDPATH += $$PWD/../../../CMAKEINSTALL/opencv/include/opencv4/
#onnxruntime
win32: LIBS += -L$$PWD/Libs/onnxruntime_x64_1.18.0/lib/ -lonnxruntime
win32: INCLUDEPATH += $$PWD/Libs/onnxruntime_x64_1.18.0/include/
win32: DEPENDPATH += $$PWD/Libs/onnxruntime_x64_1.18.0/include/

unix:!macx:LIBS += -L/home/eureka/CMAKEINSTALL/onnxruntime-linux-x64-1.18.0/lib/ -lonnxruntime.so
unix:!macx:INCLUDEPATH +=/home/eureka/CMAKEINSTALL/onnxruntime-linux-x64-1.18.0/include/
unix:!macx:DEPENDPATH +=/home/eureka/CMAKEINSTALL/onnxruntime-linux-x64-1.18.0/include/

