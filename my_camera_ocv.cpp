#include "my_camera_ocv.h"
#include <QDebug>


my_camera_ocv::my_camera_ocv(QObject *parent)
    : QObject{parent},_cutRect(0,0,0,0)
{
    fullOut();
    _DB_ind = 0;
    _mirror = false;
    _rota = 0;
    _cap = nullptr;
    connect(this,SIGNAL(getFrame(cv::Mat*)),this,SLOT(readFrame(cv::Mat*)));
    thread = new QThread();
    moveToThread(thread);
    thread->start();
//    changeDev("F:/demo.mp4");
}

my_camera_ocv::~my_camera_ocv()
{
    thread->deleteLater();
}

void my_camera_ocv::fullOut()
{
    _cutvideo = false;
}

void my_camera_ocv::cutOut()
{
    _cutvideo = true;
}

void my_camera_ocv::cutRect(int x, int y, int width, int height)
{
    _cutRect = cv::Rect(x,y,width,height);
}

cv::Mat *my_camera_ocv::GetFrame()
{
    QMetaObject::invokeMethod(this,[&](){
        if(++_DB_ind>=2)_DB_ind=0;
        emit getFrame(_DB_frame+_DB_ind);
    },Qt::QueuedConnection);
    return _DB_frame+_DB_ind;
}

bool my_camera_ocv::ready()
{
    return _cap->isOpened();
}

void my_camera_ocv::stop()
{
    blockSignals(true);
    if(_cap&&_cap->isOpened()){
        _cap->release();
//        qDebug()<<"11"<<_cap->isOpened();
//        delete _cap;
//        _cap = nullptr;
    }
    blockSignals(false);
}

bool my_camera_ocv::changeDev(const cv::String &t)
{
    blockSignals(true);
    if(_cap&&_cap->isOpened()){
        _cap->release();
        delete _cap;
    }
    _cap = new cv::VideoCapture(t);
    blockSignals(false);
    return _cap->isOpened();
}

bool my_camera_ocv::changeDev(const int &t)
{
    blockSignals(true);
    if(_cap&&_cap->isOpened()){
        _cap->release();
        delete _cap;
    }
    _cap = new cv::VideoCapture(t);
    _cap->set(cv::CAP_PROP_FRAME_WIDTH,1280);
    _cap->set(cv::CAP_PROP_FRAME_HEIGHT,720);
    blockSignals(false);
    return _cap->isOpened();
}

void my_camera_ocv::readFrame(cv::Mat*m)
{
    if(!_cap||!_cap->isOpened()){
        return;
    }
    _cap->read(*m);
    if(m->empty()){
       return;
    }
    if(_rota != 0){
        cv::rotate(*m,*m,_rota-1);
    }
    if(_mirror){
        cv::flip(*m,*m,0);
    }
    if(_cutvideo&&_cutRect.width!=0){
        *m = (*m)(_cutRect);
    }
}
