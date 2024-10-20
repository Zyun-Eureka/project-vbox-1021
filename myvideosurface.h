#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QObject>
#include <QWidget>

class myVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit myVideoSurface(QWidget* widget,QObject *parent = nullptr);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType=QAbstractVideoBuffer::NoHandle)const;
    bool present(const QVideoFrame &);
    QVideoFrame frame;
private:
    QWidget *widget;
};

#endif // MYVIDEOSURFACE_H
