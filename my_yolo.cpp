#include "my_yolo.h"
#include <QDebug>

my_yolo::my_yolo(QObject *parent)
    : QObject{parent}
{
    _maxfps = 60;
    _minfps = 1;
    _cur_fps = _minfps;
    _frame_delay = 1000.0/_cur_fps;
    _enable = false;

    _thread = new QThread();
    moveToThread(_thread);
    _thread->start();

    _mco = nullptr;

    _session = new Yolov5Session();
    _session->Initialize("models/x_ray.onnx");

    QMetaObject::invokeMethod(this,[&](){
        _frame_timer = new QTimer();
        connect(_frame_timer,SIGNAL(timeout()),this,SLOT(run()));
        _frame_timer->start(_frame_delay);
    },Qt::QueuedConnection);
//    _result_buffer = std::vector<ResultNode>();
}

my_yolo::~my_yolo()
{
    _frame_timer->stop();
}

void my_yolo::link(my_camera_ocv *i)
{
    _mco = i;
}

void my_yolo::run()
{
    if(!_mco)return;
    _start = _ela.elapsed();
    //
    _run_mat = _mco->GetFrame();
    if(_run_mat->empty()){
        return;
    }
    if(_enable){
       auto result = _session->Detect(*_run_mat);
       _result_buffer = result;
    }
    _img_buffer = QImage(_run_mat->data, _run_mat->cols, _run_mat->rows,_run_mat->step, QImage::Format_BGR888);
    emit readReady();
    _end = _ela.elapsed() - _start;
    // dynamic control

    if(_end > _frame_delay){
        _cur_fps = qMax(_minfps,_cur_fps-1);
    }else if(_end < _frame_delay){
        if(_maxfps <= _cur_fps){
            return;
        }
        _cur_fps = qMin(_maxfps,_cur_fps+1);
    }
    _frame_delay = 1000.0/_cur_fps;
    _frame_timer->setInterval(_frame_delay);
}
