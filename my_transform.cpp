#include "my_transform.h"
#include <QDebug>

#include "Yolov5Session.h"
#include "Mics.h"
#include "YoloDefine.h"

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
//    cv::imshow("img",ssMat);
    ISession* session = new Yolov5Session();
    bool isValid = session->Initialize("models/x_ray.onnx");
    auto* model = session->GetModel();
    auto result = session->Detect(ssMat);
    qDebug()<<"len:"<<result.size();

//    qDebug()<<result.;
    for (const auto& det : result)
    {
        qDebug()<<det.classIdx<<det.x<<det.y;
//        qDebug()<<
//        qDebug() << "x,y:" << det.x << " " << det.y << "w,h:" << det.w << " " << det.h << "conf:" << det.confidence << "classIdx:" << model->labels[det.classIdx] << "\n";
    }
}
