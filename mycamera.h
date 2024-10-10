#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QWidget>

#include <QCamera>

namespace Ui {
class myCamera;
}

class myCamera : public QWidget
{
    Q_OBJECT

public:
    explicit myCamera(QWidget *parent = nullptr);
    ~myCamera();

private:
    Ui::myCamera *ui;
};

#endif // MYCAMERA_H
