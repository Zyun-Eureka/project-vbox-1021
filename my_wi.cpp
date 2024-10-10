#include "my_wi.h"

my_wi::my_wi(QWidget *parent)
    : QWidget{parent}
{
    installEventFilter(this);
//    _oimage = QImage("F://imge.jpg");

}

bool my_wi::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::Paint){
        QPainter pa(this);
        QPen pen;
        pen.setColor(Qt::lightGray);
        pen.setWidth(1);
        pa.setPen(pen);
        pa.drawRect(0,0,width()-pen.width()*2,height()-pen.width()*2);
        if(_image.isNull())return false;
        pa.drawImage(_ix,_iy,_image);
    }else if(event->type()==QEvent::Resize){
        if(_oimage.isNull())return false;
        _image = _oimage.scaled(size(),Qt::KeepAspectRatio);
        _ix = (width()-_image.width())/2.0;
        _iy = (height()-_image.height())/2.0;
        update();
    }
    return QWidget::eventFilter(watched,event);;
}

void my_wi::u_img(QImage i)
{
    if(i.isNull())return;
    _oimage = i;
    _image = _oimage.scaled(size(),Qt::KeepAspectRatio);
    _ix = (width()-_image.width())/2.0;
    _iy = (height()-_image.height())/2.0;
    update();
}
