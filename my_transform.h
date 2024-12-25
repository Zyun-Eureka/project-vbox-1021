#ifndef MY_TRANSFORM_H
#define MY_TRANSFORM_H

#include <QObject>
#include <QImage>
#include <QThread>

//opencv
#include "opencv2/opencv.hpp"

#include "Yolov5Session.h"
#include "Mics.h"
#include "YoloDefine.h"
#include <QMutex>

class my_transform : public QObject
{
    Q_OBJECT
public:
    explicit my_transform(my_transform*,QObject *parent = nullptr);
    ~ my_transform();
    QImage img;
    QMutex mutex;
    my_transform *next;

    void run(QImage);
signals:
    void runs();
    void ready(QImage);
public slots:
    void GetImgs(QImage);
    void GetImg();
private:
    QThread *_myThread;
    cv::Mat ssMat;


    ISession* session;
};

#endif // MY_TRANSFORM_H
