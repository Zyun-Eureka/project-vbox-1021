#ifndef MY_WI_H
#define MY_WI_H

#include <QWidget>
#include <QEvent>
#include <QPainter>
#include <QImage>

#include "my_img.h"

class my_wi : public QWidget
{
    Q_OBJECT
public:
    explicit my_wi(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event);
    void setImgs(QList<my_img *> *);
    void setindex(int index);
public slots:
    void readImg(int img_i);
signals:
    void click_index(int inex);
private:

    QBrush bkg;
    void _reisze();
    QList<my_img*> *lists;
    my_img* _cimage;
    QImage _image;
    double _ix;
    double _iy;

    int _ind;
    int _rind;
    int _nind;
    bool click;
};

#endif // MY_WI_H
