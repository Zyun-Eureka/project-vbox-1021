#include "my_transform.h"
#include <QDebug>

my_transform::my_transform(QObject *parent)
    : QObject{parent}
{
    _myThread = new QThread();
    moveToThread(_myThread);
    _myThread->start();
}

my_transform::~my_transform()
{
    _myThread->quit();
    _myThread->wait();
    delete _myThread;
}

//Qimage 转换 Mat 方法参考 https://www.kancloud.cn/digest/usingopencv/145304

void my_transform::GetImg(QImage image)
{
    ssMat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
    //ssMat is ready
    cv::imshow("img",ssMat);
}
