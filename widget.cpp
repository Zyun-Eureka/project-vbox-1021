#include "widget.h"
#include "ui_widget.h"
#include <QInputDialog>

#include <QMouseEvent>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // vlc init
    _instance = new VlcInstance(VlcCommon::args(),this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);
    connect(_player,SIGNAL(stateChanged()),this,SLOT(vlc_stateChanged()));
    connect(_player,SIGNAL(error()),this,SLOT(vlcerror()));
    // vlc info
    _info = new QWidget(this);
    _info->show();
    _tcount = -1;
    _pflag = false;
    // info timer
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    timer->start(1000);

    // img list init
    reader = new img_reader();
    thread = new QThread(this);
    reader->setnum(4);
    reader->moveToThread(thread);
    connect(reader,SIGNAL(readReady(int,QList<QImage> *)),this,SLOT(readimg(int,QList<QImage> *)));
    thread->start();
    reader->start();

    // flist init
    flist = new funlist(this);
    flist->setGeometry(width()-40,0,flist->width(),height());
    flist->show();
    // flsit animation init
    pa = new QPropertyAnimation(flist,"pos");
    pa->setDuration(150);
    connect(pa,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),this,SLOT(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));
    state = false;
    rs = false;
    updatepa();

    //
    installEventFilter(this);
    flist->installEventFilter(this);
    _info->installEventFilter(this);
    ui->video->installEventFilter(this);
    ui->label->installEventFilter(this);

}

Widget::~Widget()
{
    _deleteBefore();
    delete timer;
    delete reader;
    delete thread;
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this){
        if(event->type()==QEvent::Resize){
            if(pa->state()==QAbstractAnimation::Stopped){
                updatepa();
            }else{
                rs = true;
            }
        }
    }else if(watched == flist){
        if(event->type()==QEvent::Enter){
            if(pa->state()==QAbstractAnimation::Stopped&&!state){
                pa->start();
                state = true;
            }
        }else if(event->type()==QEvent::Leave){
            if(pa->state()==QAbstractAnimation::Stopped&&state){
                pa->start();
                state = false;
            }
        }
    }else if(watched == _info){
        if(event->type()==QEvent::Paint){
            QPainter pa(_info);
            pa.setPen(Qt::white);
            if(_player->state()==Vlc::State::Idle){
                pa.drawText(QRectF(0,0,_info->width(),_info->height()),Qt::AlignCenter,"无视频");
            }else if(_player->state()==Vlc::State::Opening){
                pa.drawText(QRectF(0,0,_info->width(),_info->height()),Qt::AlignCenter,"[连接中]");
            }else if(_player->state()==Vlc::State::Error){
                pa.drawText(QRectF(0,0,_info->width(),_info->height()),Qt::AlignCenter,QString("[连接到　%1　异常]\n将在 %2 秒后重试").arg(url).arg(_tcount));
            }
        }
    }else if(watched == ui->video){
        if(event->type()==QEvent::Resize){
            _info->setGeometry(QRect(ui->video->mapTo(this,QPoint(0,0)),ui->video->size()));
        }
    }else if(watched == ui->label){
        if(event->type()==QEvent::ContextMenu){
            url = QInputDialog::getText(this,"设置地址","请输入摄像头地址(rtsp://XXXX:XX/path)");
            if (url.isEmpty())
                return true;
            newconnect();
        }else if(event->type()==QEvent::MouseButtonDblClick){
            if(windowState()!=Qt::WindowFullScreen){
                setWindowState(Qt::WindowFullScreen);
            }else{
                setWindowState(Qt::WindowNoState);
            }
        }
    }
    return QWidget::eventFilter(watched,event);
}

void Widget::stateChanged(QAbstractAnimation::State news, QAbstractAnimation::State olds)
{
    if(news == QAbstractAnimation::Stopped){
        if(!state){
            pa->setDirection(QAbstractAnimation::Forward);
        }else{
            pa->setDirection(QAbstractAnimation::Backward);
        }
        if(rs){
            rs = false;
            updatepa();
        }
    }
}

void Widget::vlc_stateChanged()
{
    ui->video->update();
    if(_player->state()==Vlc::State::Playing){
        _info->hide();
        if(_player->hasVout()&&!_pflag){
            _pflag = true;
            newconnect();
        }
    }else if(_player->state()==Vlc::State::Error){
        _info->show();
        _pflag = false;
        _tcount = 5;
    }
}

void Widget::timeOut()
{
    if(_tcount>0){
        _tcount--;
        _info->update();
    }else if(_tcount == 0){
        _tcount = -1;
        newconnect();
    }
}

void Widget::readimg(int head_i,QList<QImage> *list)
{
    ui->img_1->u_img((*list)[head_i%4]);
    ui->img_2->u_img((*list)[(head_i+3)%4]);
    ui->img_3->u_img((*list)[(head_i+2)%4]);
    ui->img_4->u_img((*list)[(head_i+1)%4]);
}

void Widget::vlcerror()
{
}


void Widget::updatepa()
{
    if(state){
        flist->setGeometry(width()-flist->width(),0,width()*0.25,height());
    }else{
        flist->setGeometry(width()-40,0,width()*0.25,height());
    }
    pa->setStartValue(flist->pos());
    pa->setEndValue(QPoint(width()-flist->width(),0));
}

void Widget::newconnect()
{
    _media = new VlcMedia(url, _instance);
//    _media->setOption(":network-caching=2000");
    _media->setOption(":ffmpeg-hw=1");
    _media->setOption(":vout=opengl:sout-all:sout-keep");
    _player->open(_media);
}

void Widget::_deleteBefore()
{
    timer->stop();
    reader->stop();
    thread->quit();
    thread->wait();
}
