#include "my_transform.h"
#include <QDebug>

#include <QPainter>
#include <QThread>


my_transform::my_transform(my_transform *next_tf, QObject *parent)
    : QObject{parent},next(next_tf)
{
    _myThread = new QThread();
    moveToThread(_myThread);
    _myThread->start();

    session = new Yolov5Session();
    session->Initialize("models/x_ray.onnx");

    connect(this,SIGNAL(runs()),this,SLOT(GetImg()));
}

my_transform::~my_transform()
{
    _myThread->quit();
    _myThread->wait();
    delete _myThread;
}

void my_transform::run(QImage i)
{
    img = i;
    emit runs();
}

void my_transform::GetImgs(QImage i)
{
    run(i);
}

//Qimage 转换 Mat 方法参考 https://www.kancloud.cn/digest/usingopencv/145304

void my_transform::GetImg()
{
    //    mutex.lock();
    ssMat = cv::Mat(img.height(), img.width(), CV_8UC4, (void*)img.constBits(), img.bytesPerLine());
    //ssMat is ready
    auto result = session->Detect(ssMat);
    for (const auto& det : result)
    {
        QPainter pa(&img);
        pa.setPen(Qt::red);
        pa.drawRect(det.x,det.y,det.w,det.h);
    }
    emit ready(img);
    //    cv::imshow("img",cv::Mat(img.height(), img.width(), CV_8UC4, (void*)img.constBits(), img.bytesPerLine()));
    //    mutex.unlock();
}
