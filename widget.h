#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>


// VLC-Qt
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Common.h>
//
#include "funlist.h"
#include "img_reader.h"
#include "my_wi.h"
#include <QThread>



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
    void newconnect();
    void _deleteBefore();

private slots:
    void stateChanged(QAbstractAnimation::State,QAbstractAnimation::State);


    void vlc_stateChanged();
    void timeOut();
    void readimg(int head_i, QList<QImage> *list);

    void vlcerror();

private:
    // list

    //
    bool _pflag;
    int _tcount;
    QString url;
    //

    void updatepa();

    img_reader *reader;
    QThread *thread;
    int _img_index;
    my_wi * _mtmp;
    QImage * _timg;
    QPropertyAnimation *pa;
    bool rs;
    bool state;
    funlist* flist;

    // vlc
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    //
    QWidget * _info;
    //
    QTimer *timer;

    Ui::Widget *ui;
};
#endif // WIDGET_H
