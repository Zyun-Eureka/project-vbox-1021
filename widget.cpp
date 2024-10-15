#include "widget.h"
#include "ui_widget.h"
#include <QInputDialog>

#include <QMouseEvent>
#include <QDebug>

#include <QComboBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // img list init
    reader = new img_reader();
    thread = new QThread(this);
    reader->setnum(10);
    reader->moveToThread(thread);
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
    // 动画倒计时器
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(enterWindows()));
    //
    installEventFilter(this);
    flist->installEventFilter(this);
    ui->video->installEventFilter(this);
    ui->label->installEventFilter(this);

    C = nullptr;
    P = nullptr;
    L = new QMediaPlaylist();

    listLayout = new QVBoxLayout();
    _mlist = new QWidget();
    my_wi *w;
    for(int i = 10;i>0;i--){
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
    }else if(watched == flist){
        if(event->type()==QEvent::Enter){
            // 鼠标放置后多少毫秒弹出
            timer->start(3000);
        }else if(event->type()==QEvent::Leave){
            timer->stop();
            if(pa->state()==QAbstractAnimation::Stopped&&state){
                pa->start();
                state = false;
                flist->ishow();
            }
        }
    }else if(watched == ui->video){
        if(event->type()==QEvent::Paint){
            QPainter pa(ui->video);
            pa.fillRect(ui->video->rect(),Qt::black);
        }
    }else if(watched == ui->label){
        if(event->type()==QEvent::ContextMenu){
            // 标题右键弹窗
            QDialog *d = new QDialog();
            d->setGeometry(x(),y(),200,100);
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
                    L->addMedia(url);
                    P->setVideoOutput(ui->video);
                    P->setPlaylist(L);
                    P->play();
                }
            });
            bt = new QPushButton(d);
            bt->setGeometry(125,60,50,30);
            bt->setText("确定");
            box->setGeometry(25,20,150,30);
            connect(bt,&QPushButton::released,[=](){
                d->close();
                if(C!=nullptr){
                    C->stop();
                    delete C;
                    C=nullptr;
                }
                C = new QCamera(QCameraInfo::availableCameras()[box->currentIndex()],this);
                C->setViewfinder(ui->video);
                C->start();
            });
            for(QCameraInfo i:QCameraInfo::availableCameras()){
                box->addItem(i.description());
            }
            d->setWindowTitle("选择相机");
            d->exec();
            delete bt;
            delete box;
            delete d;
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

void Widget::enterWindows()
{
    timer->stop();
    if(pa->state()==QAbstractAnimation::Stopped&&!state){
        pa->start();
        state = true;
        flist->ihide();
    }
}

//重置右侧列表坐标
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

void Widget::_deleteBefore()
{
    reader->stop();
    thread->quit();
    thread->wait();
}
