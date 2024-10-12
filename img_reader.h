#ifndef IMG_READER_H
#define IMG_READER_H

#include <QObject>

#include <QApplication>
#include <QDir>
#include <QFileSystemWatcher>

#include <QTimer>

#include "my_img.h"
#include <QFile>

#include <QThread>
#include <QDateTime>

class sig_thread: public QObject{
    Q_OBJECT
public:
    sig_thread(){
        thread = new QThread();
        moveToThread(thread);
        thread->start();
    };
    ~sig_thread(){
        thread->quit();
        thread->wait();
//        thread->deleteLater();
        delete thread;
    }
signals:
    void _sig_img_change(const my_img*);
private:
    QThread* thread;
};


class img_reader : public QObject
{
    Q_OBJECT
public:
    explicit img_reader(QObject *parent = nullptr);
    ~img_reader();


    void setnum(int i);

    void start();
    void stop();

    QList<my_img*>* getList();

signals:
//    void _sig_img_change(const my_img*);
    void readReady(int index);
    void next();
//    void readReady(int head_i,QList<QImage> *list);
    void run();
//    void clickImg(QImage*);
    void click_index(int inex);
public slots:
//    void cimgc(QImage*);
    void c_index(int i);
private slots:
    void loop();
private:
    sig_thread *_sig_thread;

    bool state;
    QList<my_img*> img_lists;
    QStringList filter;
    QStringList plist;
    QDir _idir;
    QDir _odir;
    QFile _file;
    int num;

    int index;
    int cindex;

};

#endif // IMG_READER_H
