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
#include "cutboxwin.h"

#include "my_camera_ocv.h"
#include "my_yolo.h"

#include "my_cut_config.h"

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

signals:
    void ssReady(QImage);
private slots:
    void stateChanged(QAbstractAnimation::State,QAbstractAnimation::State);
    void listShow(bool);
//    void checklist();
    void getimage(QImage i);

    //
    void getFunListSetting(int,bool,double);
    void drawimage();
private:
//    QList<rsi> _rsis;

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

//    my_threadpool *tpool;
    my_transform *mytf;

    CutBoxWin *_cutB;
    void setimg(QImage &img);
    // cutbox resize flage
    bool _cut_rs;
    // tmp
    int opencount;

    // config
    QMatrix _rotationMatrix;
    bool _video_radiu;
    bool _video_mirror;
    bool _video_opencv;



    bool screenshot;

    //
    my_camera_ocv *mco;
    my_yolo *myo;
    bool updateflage;
    QFont vfont;
    QPen vpen;
    my_cut_config _mcc;
    QImage vimg;
//    QImage video_buffer;
    Ui::Widget *ui;
};
#endif // WIDGET_H
