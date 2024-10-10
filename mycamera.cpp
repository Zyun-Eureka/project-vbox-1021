#include "mycamera.h"
#include "ui_mycamera.h"

#include <QDebug>
#include <QCameraInfo>

//#include <QMediaDevices>


myCamera::myCamera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myCamera)
{
    ui->setupUi(this);

//    QCamera *c = new QCamera(this);
    qDebug()<<QCameraInfo::availableCameras();
}

myCamera::~myCamera()
{
    delete ui;
}
