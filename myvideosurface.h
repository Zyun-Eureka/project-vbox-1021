#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QObject>
#include <QWidget>
#include <QTime>
#include <QDebug>

#include <sys/timeb.h>

//#define delays 100

class myVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit myVideoSurface(QWidget* widget,QObject *parent = nullptr);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType=QAbstractVideoBuffer::NoHandle)const;
    bool present(const QVideoFrame &);
    // change output
    void ChangeWidget(QWidget*);

    void startcut();
    void endcut();

    // frame
    QVideoFrame frame;
    // fps delays
    int delays;

signals:
    void imgready(QImage);
private:
    QWidget *widget;
    timeb curtime;
    timeb tmptime;
};

#endif // MYVIDEOSURFACE_H
