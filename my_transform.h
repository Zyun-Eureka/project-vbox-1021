#ifndef MY_TRANSFORM_H
#define MY_TRANSFORM_H

#include <QObject>
#include <QImage>
#include <QThread>

//opencv
#include <opencv2/opencv.hpp>

class my_transform : public QObject
{
    Q_OBJECT
public:
    explicit my_transform(QObject *parent = nullptr);
    ~ my_transform();

public slots:
   void GetImg(QImage);
signals:
private:
   QThread *_myThread;
   cv::Mat ssMat;
};

#endif // MY_TRANSFORM_H
