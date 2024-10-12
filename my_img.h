#ifndef MY_IMG_H
#define MY_IMG_H

#include <QObject>
#include <QImage>
#include <QWidget>

class my_img : public QObject
{
    Q_OBJECT
public:
    explicit my_img(QObject *parent = nullptr);
    QImage img;
    bool click;

    QWidget *wi;
    void clicked();
signals:
    void _sig_clicked(const my_img*);
public slots:
    void _slot_clicked(const my_img*);

};

#endif // MY_IMG_H
