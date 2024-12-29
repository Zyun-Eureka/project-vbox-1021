QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cutboxwin.cpp \
    funlist.cpp \
    img_reader.cpp \
    main.cpp \
    my_camera_ocv.cpp \
    my_close.cpp \
    my_cut_config.cpp \
    my_img.cpp \
    my_wi.cpp \
    my_yolo.cpp \
    widget.cpp \
    yolo/ModelParser.cpp \
    yolo/ModelProcessor.cpp \
    yolo/Yolov5Session.cpp

HEADERS += \
    cutboxwin.h \
    funlist.h \
    img_reader.h \
    my_camera_ocv.h \
    my_close.h \
    my_cut_config.h \
    my_img.h \
    my_wi.h \
    my_yolo.h \
    widget.h \
    yolo/ISession.h \
    yolo/Mics.h \
    yolo/Model.h \
    yolo/ModelParser.h \
    yolo/ModelProcessor.h \
    yolo/YoloDefine.h \
    yolo/Yolov5Session.h

FORMS += \
    cutboxwin.ui \
    funlist.ui \
    my_cut_config.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#encoding
QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8

#opencv
CONFIG(debug, debug|release) {
    message("debug")
    win32: LIBS += $$PWD/Libs/opencv_4.2_debug/lib/opencv_*
    win32: INCLUDEPATH += $$PWD/Libs/opencv_4.2_debug/include/
    win32: DEPENDPATH += $$PWD/Libs/opencv_4.2_debug/include/
}

CONFIG(release, debug|release) {
    message("release")
    win32: LIBS += $$PWD/Libs/opencv_4.2/lib/opencv_*
    win32: INCLUDEPATH += $$PWD/Libs/opencv_4.2/include/
    win32: DEPENDPATH += $$PWD/Libs/opencv_4.2/include/

    unix:!macx:LIBS += -L$$PWD/../../../CMAKEINSTALL/opencv/lib/ -lopencv*
    unix:!macx:INCLUDEPATH += $$PWD/../../../CMAKEINSTALL/opencv/include/opencv4/
    unix:!macx:DEPENDPATH += $$PWD/../../../CMAKEINSTALL/opencv/include/opencv4/
}


#onnxruntime
win32: LIBS += -L$$PWD/Libs/onnxruntime_x64_1.18.0/lib/ -lonnxruntime
win32: INCLUDEPATH += $$PWD/Libs/onnxruntime_x64_1.18.0/include/
win32: DEPENDPATH += $$PWD/Libs/onnxruntime_x64_1.18.0/include/

unix:!macx:LIBS += -L/home/eureka/CMAKEINSTALL/onnxruntime-linux-x64-1.18.0/lib/ -lonnxruntime.so
unix:!macx:INCLUDEPATH +=/home/eureka/CMAKEINSTALL/onnxruntime-linux-x64-1.18.0/include/
unix:!macx:DEPENDPATH +=/home/eureka/CMAKEINSTALL/onnxruntime-linux-x64-1.18.0/include/

