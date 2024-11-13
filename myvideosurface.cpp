#include "myvideosurface.h"

myVideoSurface::myVideoSurface(QWidget *widget, QObject *parent)
    : QAbstractVideoSurface(parent),widget(widget)
{

}

QList<QVideoFrame::PixelFormat> myVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    return QList<QVideoFrame::PixelFormat>()<<QVideoFrame::Format_RGB32<< QVideoFrame::Format_ARGB32;
}

bool myVideoSurface::present(const QVideoFrame &frame)
{
    if (frame.isValid()) {
        this->frame = frame;
        widget->update();
        emit imgready(frame.image());
        return true;
    }
    return false;
}
