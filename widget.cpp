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
    vtimer = new QTimer(this);
    mytf = new my_transform();
    connect(vtimer,SIGNAL(timeout()),this,SLOT(checklist()));
    connect(this,SIGNAL(ssReady(QImage)),mytf,SLOT(GetImg(QImage)));
    vtimer->start(10);
    pen.setColor(Qt::red);
    font.setPixelSize(20);
    screenshot = false;


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
                QVideoFrame frame = myvs->frame;
                frame.map(QAbstractVideoBuffer::ReadOnly);
                QImage image;
                if(screenshot){
                    screenshot = false;
                    //Tips: 因为图像解码完后在这里会压缩到合适的大小再绘制到控件上，所以当有截图任务时会将原始图像传过去在压缩
                    image=QImage(frame.bits(),frame.width(),frame.height(),frame.bytesPerLine(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
                    emit ssReady(image);
                    image=image.scaled(ui->video->size(),Qt::KeepAspectRatio);
                }else{
                    image=QImage(frame.bits(),frame.width(),frame.height(),frame.bytesPerLine(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())).scaled(ui->video->size(),Qt::KeepAspectRatio);
                }
                frame.unmap();
                pa.drawImage((ui->video->width()-image.width())/2.0,(ui->video->height()-image.height())/2.0,image);
            }
            if(!_rsis.isEmpty()){
                pa.setPen(pen);
                pa.setFont(font);
                for(rsi &i:_rsis){
                    pa.drawRect(i.rect);
                    pa.drawText(i.rect.x(),i.rect.y()+font.pixelSize(),i.text);
                }
            }
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
