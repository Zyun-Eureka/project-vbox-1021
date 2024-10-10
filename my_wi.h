#ifndef MY_WI_H
#define MY_WI_H

#include <QWidget>
#include <QEvent>
#include <QPainter>

class my_wi : public QWidget
{
    Q_OBJECT
public:
    explicit my_wi(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event);

    void u_img(QImage i);
private:
    QImage _oimage;
    QImage _image;
    double _ix;
    double _iy;
};

#endif // MY_WI_H
