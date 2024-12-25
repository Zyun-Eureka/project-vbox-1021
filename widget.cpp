#include "widget.h"
#include "ui_widget.h"
#include <QInputDialog>

#include <QMouseEvent>
#include <QDebug>
#include <QComboBox>

#include <QGraphicsVideoItem>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);



    // img list init
    reader = new img_reader();
    thread = new QThread(this);
    reader->setnum(5);
    reader->moveToThread(thread);
    thread->start();
    reader->start();

    // flist init
    flist = new funlist(this);
    flist->setGeometry(width()-40,0,flist->width(),height());
    flist->show();
    connect(flist,SIGNAL(setting(int,bool,double)),this,SLOT(getFunListSetting(int,bool,double)));
    connect(flist,SIGNAL(listShow(bool)),this,SLOT(listShow(bool)));
    _video_radiu = false;
    _video_mirror = false;
    _rotationMatrix.rotate(0);
    // flsit animation init
    pa = new QPropertyAnimation(flist,"pos");
    pa->setDuration(150);
    connect(pa,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),this,SLOT(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));
    state = false;
    rs = false;
    updatepa();

    //register listener
    installEventFilter(this);
    flist->installEventFilter(this);
    ui->video->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->result->installEventFilter(this);


    //file & camere decoding
    C = nullptr;
    P = nullptr;
    L = new QMediaPlaylist();
    //video decoding & opencv
    myvs = new myVideoSurface(ui->video);
    connect(myvs,SIGNAL(imgready(QImage)),this,SLOT(getimage(QImage)));
    mytf = new my_transform(nullptr);
    connect(this,SIGNAL(ssReady(QImage)),mytf,SLOT(GetImgs(QImage)));
    connect(mytf,&my_transform::ready,[&](QImage i){
        if(_video_opencv){
        _opc_img = i;
        ui->video->update();
        }
    });

    //img list layout
    listLayout = new QVBoxLayout();
    _mlist = new QWidget();
    my_wi *w;
    for(int i = 5;i>0;i--){
        w = new my_wi();
        lists.push_back(w);
        listLayout->addWidget(w);
        connect(reader,SIGNAL(readReady(int)),w,SLOT(readImg(int)));
        w->setindex(i);
        w->setImgs(reader->getList());
    }
    _mlist->setLayout(listLayout);
    ui->list->setWidget(_mlist);
    ui->list->setWidgetResizable(true);
    ui->list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->splitter_2->setSizes({800,200});

    //cutbox
    _cutB = new CutBoxWin();
    _cutB->installEventFilter(this);
    _cut_rs = false;

    opencount = 0;
    _video_opencv =false;
}

Widget::~Widget()
{
    _deleteBefore();
    delete reader;
    delete thread;
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this){
        // resize event
        if(event->type()==QEvent::Resize){
            if(pa->state()==QAbstractAnimation::Stopped){
                updatepa();
            }else{
                rs = true;
            }
        }
    }else if(watched == ui->video){
        // paint event
        if(event->type()==QEvent::Paint){
            if(!_video_opencv){
                if(_timg.isNull())return true;
                QPainter pa(ui->video);
                QImage i = _timg;
                if(_cutB->cutRect.x()!=-1){
                    i = i.copy(_cutB->cutRect);
                }
                i = i.scaled(ui->video->size(),Qt::KeepAspectRatio);
                pa.drawImage((ui->video->width()-i.width())/2.0,(ui->video->height()-i.height())/2.0,i);
            }else{
                if(_opc_img.isNull())return true;
                QPainter pa(ui->video);
                QImage i = _opc_img;

                pa.drawImage((ui->video->width()-i.width())/2.0,(ui->video->height()-i.height())/2.0,i);
            }
        }
    }else if(watched == ui->label){
        if(event->type()==QEvent::ContextMenu){
            _cut_rs = false;
            QDialog *d = new QDialog();
            d->setGeometry(x(),y()+30,200,100);
            QComboBox * box = new QComboBox(d);
            QPushButton *bt = new QPushButton(d);
            bt->setGeometry(25,60,90,30);
            bt->setText("从文件播放");
            connect(bt,&QPushButton::released,[=](){
                if(C!=nullptr){
                    C->stop();
                    delete C;
                    C=nullptr;
                }
                if(P!=nullptr){
                    P->stop();
                    delete P;
                    P=nullptr;
                }
                L->clear();
                QUrl url = QFileDialog::getOpenFileUrl(nullptr,"选择要播放的视频",QUrl(),"mp4视频文件 (*.mp4)");
                if(!url.isEmpty()){
                    d->close();
                    P = new QMediaPlayer();

                    connect(P,&QMediaPlayer::stateChanged,[=](QMediaPlayer::State s){
                        if(s==QMediaPlayer::StoppedState){
                            P->play();
                        }
                    });
                    L->addMedia(url);
                    P->setVideoOutput(myvs);
                    P->setPlaylist(L);
                    P->play();
                }
            });
            QPushButton * bt1 = new QPushButton(d);
            bt1->setGeometry(125,60,50,30);
            bt1->setText("确定");
            box->setGeometry(25,20,150,30);
            connect(bt1,&QPushButton::released,[=](){
                d->close();
                if(C!=nullptr){
                    C->stop();
                    delete C;
                    C=nullptr;
                }
                if(P!=nullptr){
                    P->stop();
                    delete P;
                    P=nullptr;
                }
                C = new QCamera(QCameraInfo::availableCameras()[box->currentIndex()],this);
                C->setViewfinder(myvs);
                C->start();
            });
            for(QCameraInfo i:QCameraInfo::availableCameras()){
                box->addItem(i.description());
            }
            d->setWindowTitle("选择相机");
            d->exec();
            delete bt;
            delete bt1;
            delete box;
            delete d;
        }else if(event->type()==QEvent::MouseButtonDblClick){
            screenshot = true;
        }
    }else if(watched == ui->result){
        if(event->type()==QEvent::Resize){
            ui->result->setMinimumHeight(ui->result->width()/2.0);
        }
    }else if(watched == _cutB){
        if(event->type()==QEvent::Paint){
            QPainter pa(_cutB);
            if(!_cut_rs){
                _cutB->setMaximumSize(_timg.size());
                _cutB->setMinimumSize(_timg.size());
                _cut_rs = true;
            }
            pa.drawImage(0,0,_timg);
        }else if(event->type()==QEvent::Hide){
            myvs->ChangeWidget(ui->video);
        }
    }
    return QWidget::eventFilter(watched,event);
}

// pop windows animation event
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

// animation control
void Widget::listShow(bool b)
{
    if(pa->state()!=QAbstractAnimation::Stopped){
        pa->stop();
    }
    pa->start();
    state = b;
}

// tmp func for draw line
void Widget::checklist()
{
    if(_rsis.isEmpty())return;
    QDateTime ct = QDateTime::currentDateTime();
    int len = 0;
    for(rsi &i:_rsis){
        if(i.time>ct){
            break;
        }
        len++;
    }
    if(len!=0){
        _rsis = _rsis.mid(len);
        ui->video->update();
    }
}

void Widget::getimage(QImage i)
{
    _timg = i;
    setimg(_timg);
    if(_cutB->isActiveWindow()){
        _cut_rs = false;
        _cutB->update();
    }else if(_video_opencv){
        opencount -=1;
        if(opencount<=0)_video_opencv = false;
        if(_cutB->cutRect.x()!=-1){
            _timg = _timg.copy(_cutB->cutRect);
        }
        _timg = _timg.scaled(ui->video->size(),Qt::KeepAspectRatio);
        emit ssReady(_timg);
//        qDebug()<<opencount;
    }else{
        qDebug()<<"1";
        ui->video->update();
    }
}

void Widget::getFunListSetting(int i, bool b, double d)
{
    if(i==1){
        _cutB->show();
    }else if(i==2){
        // mirror
        _video_mirror = b;
    }else if(i==3){
        // radius
        _cut_rs = false;
        _video_radiu = d!=0;
        _rotationMatrix.rotate(d);
    }else if(i==4){
        // start opencv
        _video_opencv = b;
        opencount = 1000/myvs->delays*10;

    }else if(i==5){
        // limit video fps
        if(d==0)return;
        myvs->delays = 1000/d;
    }

}

//重置右侧列表坐标
void Widget::updatepa()
{
    if(state){
        flist->setGeometry(width()-width()*0.25,0,width()*0.25,height());
        pa->setStartValue(QPoint(width()-40,0));
        pa->setEndValue(flist->pos());
    }else{
        flist->setGeometry(width()-40,0,width()*0.25,height());
        pa->setStartValue(flist->pos());
        pa->setEndValue(QPoint(width()-flist->width(),0));
    }
}

void Widget::setimg(QImage &img)
{
    if(_video_radiu){
        img = img.transformed(_rotationMatrix);
    }
    if(_video_mirror){
        img = img.mirrored();
    }
}

void Widget::_deleteBefore()
{
    reader->stop();
    thread->quit();
    thread->wait();
}
// tmp func for draw line
void Widget::addRect(QRect r, QString s, int i)
{
    rsi tr;
    tr.rect = r;
    tr.text = s;
    tr.time = QDateTime::currentDateTime().addMSecs(i);
    _rsis.push_back(tr);
}

