#ifndef MY_THREADPOOL_H
#define MY_THREADPOOL_H

#include <QObject>
#include <QThread>

#include "my_transform.h"

class my_threadpool : public QObject
{
    Q_OBJECT
public:
    explicit my_threadpool(QObject *parent = nullptr);
    ~my_threadpool();

signals:
    void ready(my_transform *);
public slots:
    void getImage(QImage);
private:
    QThread *thread;

    QVector<my_transform*> _mytf_list;
    my_transform *_mytf_now;
    my_transform *_mytf_latest;
    my_transform *_mytf_ready;
};

#endif // MY_THREADPOOL_H
