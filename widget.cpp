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
    //    _cutB->installEventFilter(this);
    //    _cut_rs = false;

    opencount = 0;
    _video_opencv =false;

    // 12/26
    mco = new my_camera_ocv();
    //    mco->cutRect(100,100,200,200);
    myo = new my_yolo();
    myo->link(mco);
    connect(myo,SIGNAL(readReady()),this,SLOT(drawimage()));
    updateflage = false;
    //    vfont.setWeight(10);
    vpen.setColor(Qt::red);
    vpen.setWidth(2);
    vfont.setPixelSize(20);
    vfont.setBold(true);

    connect(&_mcc,&my_cut_config::cutRect,this,[&](int x,int y,int w,int h){
        mco->cutRect(x,y,w,h);
    });
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
            if(updateflage)updateflage = false;
            else return true;
            if(myo->_img_buffer.isNull())return true;
            QPainter pa(ui->video);
            pa.setPen(vpen);
            pa.setFont(vfont);
            vimg = myo->_img_buffer.copy().scaled(ui->video->size(),Qt::KeepAspectRatio);
            double x = (ui->video->width()-vimg.width())/2.0;
            double y = (ui->video->height()-vimg.height())/2.0;
            double w = vimg.width()*1.0/myo->_img_buffer.width();
            double h = vimg.height()*1.0/myo->_img_buffer.height();
            pa.drawImage(x,y,vimg);
            for (const auto& det : myo->_result_buffer)
            {
                pa.drawText(x+det.x*w,y+det.y*h-vpen.width(),"手枪");
                pa.drawRect(x+det.x*w,y+det.y*h,det.w*w,det.h*h);
            }
            pa.drawText(10,30,QString("调试输出 FPS:%0 识别速度:%1ms").arg(myo->_cur_fps).arg(myo->_frame_delay));
        }
    }else if(watched == ui->label){
        //        return false;
        if(event->type()==QEvent::ContextMenu){
            _cut_rs = false;
            QDialog *d = new QDialog();
            d->setGeometry(x(),y()+30,200,100);
            QComboBox * box = new QComboBox(d);
            QPushButton *bt = new QPushButton(d);
            bt->setGeometry(25,60,90,30);
            bt->setText("从文件播放");
            connect(bt,&QPushButton::released,[=](){
                QUrl url = QFileDialog::getOpenFileUrl(nullptr,"选择要播放的视频",QUrl(),"mp4视频文件 (*.mp4)");
                if(!url.isEmpty()){
                    mco->changeDev(url.toString().toLocal8Bit().data());
                    d->close();
                }
            });
            QPushButton * bt1 = new QPushButton(d);
            bt1->setGeometry(125,60,50,30);
            bt1->setText("确定");
            box->setGeometry(25,20,150,30);
            connect(bt1,&QPushButton::released,[=](){
                d->close();
                mco->changeDev(box->currentText().toInt());
            });
            int i =0;
            mco->stop();
            for(i;i<10;i++){
                cv::VideoCapture vc(i);
                if(vc.isOpened()){
                    box->addItem(QString::number(i));
                    vc.release();
                }
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

void Widget::getimage(QImage i)
{
    //    _timg = i;
    //    setimg(_timg);
    //    if(_cutB->isActiveWindow()){
    //        _cut_rs = false;
    //        _cutB->update();
    //    }else if(_video_opencv){
    //        opencount -=1;
    //        if(opencount<=0)_video_opencv = false;
    //        if(_cutB->cutRect.x()!=-1){
    //            _timg = _timg.copy(_cutB->cutRect);
    //        }
    //        _timg = _timg.scaled(ui->video->size(),Qt::KeepAspectRatio);
    //        emit ssReady(_timg);
    //    }else{
    //        qDebug()<<"1";
    //        ui->video->update();
    //    }
}

void Widget::getFunListSetting(int i, bool b, double d)
{
    //    qDebug()<<i<<b<<d;
    if(i==1){
        //        mco->fullOut();
        //        _cutB->show();
        _mcc.show();
    }else if(i==2){
        // mirror
        //        _video_mirror = b;
        mco->setMirror(b);
    }else if(i==3){
        mco->setRate(d);
        // radius
        //        _cut_rs = false;
        //        _video_radiu = d!=0;
        //        _rotationMatrix.rotate(d);
    }else if(i==4){
        // start opencv
        //        _video_opencv = b;
        //        opencount = 1000/myvs->delays*10;
        if(b){
            myo->startcv();
        }else{
            myo->stopcv();
        }

    }else if(i==5){
        // limit video fps
        //        if(d==0)return;
        //        myvs->delays = 1000/d;
    }else if(i==6){
        if(b){
            mco->cutOut();
        }else{
            mco->fullOut();
        }
    }

}

void Widget::drawimage()
{
    ui->video->update();
    updateflage = true;
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
