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
    connect(flist,SIGNAL(listShow(bool)),this,SLOT(listShow(bool)));
    // flsit animation init
    pa = new QPropertyAnimation(flist,"pos");
    pa->setDuration(150);
    connect(pa,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),this,SLOT(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));
    state = false;
    rs = false;
    updatepa();

    installEventFilter(this);
    flist->installEventFilter(this);
    ui->video->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->result->installEventFilter(this);

    //
    C = nullptr;
    P = nullptr;
    L = new QMediaPlaylist();
    myvs = new myVideoSurface(ui->video);
    mytf = new my_transform(nullptr);
    connect(this,SIGNAL(ssReady(QImage)),mytf,SLOT(GetImgs(QImage)));
//    vtimer = new QTimer(this);

//    tpool = new my_threadpool();
//    connect(myvs,SIGNAL(imgready(QImage)),tpool,SLOT(getImage(QImage)));
//    connect(tpool,SIGNAL(ready(my_transform*)),this,SLOT(getimage(my_transform*)));
//    connect(vtimer,SIGNAL(timeout()),this,SLOT(checklist()));
//    vtimer->start(300);
//    pen.setColor(Qt::red);
//    font.setPixelSize(20);

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
    ui->splitter_2->setSizes({800,200});

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
        if(event->type()==QEvent::Resize){
            if(pa->state()==QAbstractAnimation::Stopped){
                updatepa();
            }else{
                rs = true;
            }
        }
    }else if(watched == ui->video){
        if(event->type()==QEvent::Paint){
            QPainter pa(ui->video);
            if(myvs->frame.isValid()){
                QImage image;
                QMatrix rotationMatrix;
                rotationMatrix.rotate(-90);  // 如果视频显示竖屏，需要旋转
                if(screenshot){
                    screenshot = false;
                    image = myvs->frame.image().transformed(rotationMatrix);
                    emit ssReady(image);
                    image=image.scaled(ui->video->size(),Qt::KeepAspectRatio);
                }else{
                    image = myvs->frame.image().transformed(rotationMatrix).scaled(ui->video->size(),Qt::KeepAspectRatio);
                }
                pa.drawImage((ui->video->width()-image.width())/2.0,(ui->video->height()-image.height())/2.0,image);
            }


            ////            pa.drawImage((ui->video->width()-mytf->img.width())/2.0,(ui->video->height()-mytf->img.height())/2.0,mytf->img);
            //            if(false&&myvs->frame.isValid()){
            //                QVideoFrame frame = myvs->frame;
            ////                frame.map(QAbstractVideoBuffer::ReadOnly);
            ////                qDebug()<<frame.startTime();

            //                QMatrix rotationMatrix;
            //                rotationMatrix.rotate(0);  // 如果视频显示竖屏，需要旋转
            //                QImage image;
            //                if(screenshot){
            //                    screenshot = false;
            //                    //Tips: 因为图像解码完后在这里会压缩到合适的大小再绘制到控件上，所以当有截图任务时会将原始图像传过去在压缩
            ////                    image=QImage(frame.bits(),frame.width(),frame.height(),frame.bytesPerLine(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
            //                    image = frame.image().transformed(rotationMatrix);
            ////                    image = frame.image().transformed(rotationMatrix).scaled(ui->video->size(),Qt::KeepAspectRatio);
            //                    emit ssReady(image);
            ////                    qDebug()<<image.size();
            //                    image=image.scaled(ui->video->size(),Qt::KeepAspectRatio);
            ////                    qDebug()<<image.size();
            //                }else{
            ////                    image=QImage(frame.bits(),frame.width(),frame.height(),frame.bytesPerLine(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())).scaled(ui->video->size(),Qt::KeepAspectRatio);
            //                    image = frame.image().transformed(rotationMatrix).scaled(ui->video->size(),Qt::KeepAspectRatio);
            //                }
            //                pa.drawImage((ui->video->width()-image.width())/2.0,(ui->video->height()-image.height())/2.0,image);
            //            }
        }
    }else if(watched == ui->label){
        if(event->type()==QEvent::ContextMenu){
            // 标题右键弹窗
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

void Widget::listShow(bool b)
{
    if(pa->state()!=QAbstractAnimation::Stopped){
        pa->stop();
    }
    pa->start();
    state = b;
}

void Widget::checklist()
{
    //    qDebug()<<mytf->thread()->isFinished();
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

void Widget::getimage(my_transform *i)
{

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

void Widget::_deleteBefore()
{
    reader->stop();
    thread->quit();
    thread->wait();
}

void Widget::addRect(QRect r, QString s, int i)
{
    rsi tr;
    tr.rect = r;
    tr.text = s;
    tr.time = QDateTime::currentDateTime().addMSecs(i);
    _rsis.push_back(tr);
}

//void Widget::on_checkBox_toggled(bool checked)
//{
//    if(checked){
//        //        connect(myvs,SIGNAL(imgready(QImage)),,SLOT(getImage(QImage)));
//        connect(myvs,&myVideoSurface::imgready,[=](QImage i){
//            tpool->getImage(i.scaled(ui->video->size(),Qt::KeepAspectRatio));
//        });
//    }else{
//        //        disconnect(myvs,SIGNAL(imgready(QImage)),tpool,SLOT(getImage(QImage)));
//    }
//}

