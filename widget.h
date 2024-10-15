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

private slots:
    void stateChanged(QAbstractAnimation::State,QAbstractAnimation::State);
    void enterWindows();

private:

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
    QImage * _timg;
    QPropertyAnimation *pa;
    bool rs;
    bool state;
    funlist* flist;
    //
    QTimer *timer;
    //
    QCamera *C;
    QMediaPlaylist *L;
    QMediaPlayer *P;

    Ui::Widget *ui;
};
#endif // WIDGET_H
