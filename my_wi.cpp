#include "my_wi.h"
#include <QDebug>

my_wi::my_wi(QWidget *parent)
    : QWidget{parent}
{
    installEventFilter(this);
    lists = nullptr;
    setMinimumHeight(120);
    _rind =_nind= -1;
    click = false;
    _cimage = nullptr;
    bkg.setColor(QColor("#2F343A"));
}

bool my_wi::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::Paint){
        QPainter pa(this);
        QPen pen;
        pen.setColor(Qt::lightGray);
        if(!_image.isNull()){
            pa.drawImage(0,0,_image);
            if(lists->at(_rind)->click){
                pen.setColor(Qt::red);
            }
        }else{
            pa.setBrush(bkg);
        }
        pa.setPen(pen);
        pa.drawRect(0,0,width()-pen.width()*2,height()-pen.width()*2);
    }else if(event->type()==QEvent::Resize){
        _reisze();
    }else if(event->type()==QEvent::MouseButtonPress){
        if(_cimage==nullptr)return true;
        _cimage->clicked();
    }
    return QWidget::eventFilter(watched,event);;
}

void my_wi::setImgs(QList<my_img*> *i)
{
    lists = i;
}

void my_wi::setindex(int index)
{
    _ind = index;
}

void my_wi::_reisze()
{
    if(_cimage==nullptr||_cimage->img.isNull())return;
    _image = _cimage->img.scaled(QSize(width(),1000),Qt::KeepAspectRatio);
    setMinimumHeight(_image.height());
    update();
}

void my_wi::readImg(int img_i)
{
    _rind = (img_i+_ind)%lists->count();
    if(lists!=nullptr||_rind==-1){
        _cimage = (*lists)[_rind];
        _cimage->wi = this;
        _reisze();
    }
}
