#ifndef MY_YOLO_H
#define MY_YOLO_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QImage>
#include <QElapsedTimer>
//
#include <vector>
//
#include "opencv2/opencv.hpp"
//
#include "yolo/Yolov5Session.h"
#include "yolo/Mics.h"
#include "yolo/YoloDefine.h"

#include "my_camera_ocv.h"

class my_yolo : public QObject
{
    Q_OBJECT
public:
    explicit my_yolo(QObject *parent = nullptr);
    ~my_yolo();

    QImage _img_buffer;
    std::vector<ResultNode> _result_buffer;
    int _cur_fps;
    double _frame_delay;

    void link(my_camera_ocv*);

    void startcv(){_enable = true;}
    void stopcv(){_enable = false;}
signals:
    void readReady();
private slots:
    void run();
private:
    QThread* _thread;
    QTimer* _frame_timer;
    int _maxfps;
    int _minfps;

    QElapsedTimer _ela;
    int64 _start;
    int64 _end;
    int _tar_fps;

    cv::Mat* _run_mat;

    ISession* _session;

    my_camera_ocv* _mco;
    bool _enable;
};

#endif // MY_YOLO_H
