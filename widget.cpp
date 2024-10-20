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

    //
    C = nullptr;
    P = nullptr;
    L = new QMediaPlaylist();
    myvs = new myVideoSurface(ui->video);
    vtimer = new QTimer(this);
    connect(vtimer,SIGNAL(timeout()),this,SLOT(checklist()));
    vtimer->start(10);
    pen.setColor(Qt::red);
    font.setPixelSize(20);


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
            if(myvs->frame.isValid()){
                QVideoFrame frame = myvs->frame;
                frame.map(QAbstractVideoBuffer::ReadOnly);
                QImage image = QImage(frame.bits(),frame.width(),frame.height(),frame.bytesPerLine(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())).scaled(ui->video->size(),Qt::KeepAspectRatio);
                pa.drawImage((ui->video->width()-image.width())/2.0,(ui->video->height()-image.height())/2.0,image);
                frame.unmap();
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
            QDialog *d = new QDialog();
            d->setGeometry(x(),y()+30,200,130);
            QLineEdit *e = new QLineEdit(d);
            e->setGeometry(25,20,150,30);
            e->setPlaceholderText("输入要显示的文字");
            QLineEdit* e1 = new QLineEdit(d);
            e1->setGeometry(25,55,40,30);
            e1->setPlaceholderText("X坐标");
            QLineEdit* e2 = new QLineEdit(d);
            e2->setGeometry(70,55,40,30);
            e2->setPlaceholderText("Y坐标");
            QLineEdit* e3 = new QLineEdit(d);
            e3->setGeometry(115,55,60,30);
            e3->setPlaceholderText("毫秒");
            QPushButton* bt = new QPushButton(d);
            bt->setGeometry(25,90,150,30);
            bt->setText("确定");
            connect(bt,&QPushButton::released,[=](){
                d->close();
                addRect(QRect(e1->text().toInt(),e2->text().toInt(),100,100),e->text(),e3->text().toInt());
            });
            d->exec();
            delete e1;
            delete e2;
            delete e3;
            delete bt;
            delete d;
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

void Widget::addRect(QRect r, QString s, int i)
{
    rsi tr;
    tr.rect = r;
    tr.text = s;
    tr.time = QDateTime::currentDateTime().addMSecs(i);
    _rsis.push_back(tr);
}
