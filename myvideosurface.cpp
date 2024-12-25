#include "myvideosurface.h"
myVideoSurface::myVideoSurface(QWidget *widget, QObject *parent)
    : QAbstractVideoSurface(parent)
{
    ChangeWidget(widget);
//    curtime = std::time(0);
    ftime(&curtime);
    delays = 50;
}

QList<QVideoFrame::PixelFormat> myVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    return QList<QVideoFrame::PixelFormat>()<<QVideoFrame::Format_RGB32<< QVideoFrame::Format_ARGB32;
}

bool myVideoSurface::present(const QVideoFrame &frame)
{
    if (frame.isValid()) {
        ftime(&tmptime);
        if(tmptime.time-curtime.time){
            if((tmptime.millitm+1000-curtime.millitm)<=delays)return true;
        }else{
            if(tmptime.millitm-curtime.millitm<=delays)return true;
        }
        curtime = tmptime;
        emit imgready(frame.image());
//        this->frame = frame;
//        if(_video_opencv){

//        }else{
//            widget->update();
//        }
        return true;
    }
    return false;
}

void myVideoSurface::ChangeWidget(QWidget *i)
{
    if(i==nullptr)return;
    widget = i;
}
