#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>
#include <QCamera>
#include <QCameraInfo>

#include <QMediaPlaylist>
#include <QMediaPlayer>

#include "funlist.h"
#include "img_reader.h"
#include "my_wi.h"
#include <QThread>
#include <QVBoxLayout>

#include <QFileDialog>
#include "myvideosurface.h"
#include "my_transform.h"
#include "my_threadpool.h"
#include "cutboxwin.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool eventFilter(QObject *watched, QEvent *event);
    void _deleteBefore();
    void addRect(QRect,QString,int);

    struct rsi{
        QRect rect;
        QString text;
        QDateTime time;
    };

signals:
    void ssReady(QImage);
private slots:
    void stateChanged(QAbstractAnimation::State,QAbstractAnimation::State);
    void listShow(bool);
    void checklist();
    void getimage(QImage i);

    //
    void getFunListSetting(int,bool,double);
private:
    QList<rsi> _rsis;

    bool _pflag;
    int _tcount;
    QString url;


    QWidget* _mlist;
    QList<my_wi*> lists;
    QVBoxLayout *listLayout;
    //

    void updatepa();
    img_reader *reader;
    QThread *thread;
    int _img_index;
    my_wi * _mtmp;
    QPropertyAnimation *pa;
    bool rs;
    bool state;
    //
    funlist* flist;
    //
    QTimer *vtimer;
    //
    QCamera *C;
    QMediaPlaylist *L;
    QMediaPlayer *P;
    myVideoSurface *myvs;
    QImage _timg;
    my_threadpool *tpool;
    my_transform *mytf;

    CutBoxWin *_cutB;
    void setimg(QImage &img);
    // cutbox resize flage
    bool _cut_rs;
    // tmp
    int opencount;
    QImage _opc_img;
    // config
    QMatrix _rotationMatrix;
    bool _video_radiu;
    bool _video_mirror;
    bool _video_opencv;



    QFont font;
    bool screenshot;


    Ui::Widget *ui;
};
#endif // WIDGET_H
