#ifndef MY_CAMERA_OCV_H
#define MY_CAMERA_OCV_H

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

class my_camera_ocv : public QObject
{
    Q_OBJECT
public:
    explicit my_camera_ocv(QObject *parent = nullptr);
    ~my_camera_ocv();
    //调整为完整输出
    void fullOut();
    //调整为裁切输出
    void cutOut();
    void cutRect(int x,int y,int width,int height);
    static void Transform(cv::Mat m,QImage &i);
    cv::Mat* GetFrame();
    bool ready();

    void stop();
    bool changeDev(const cv::String &t);
    bool changeDev(const int &t);

    void setMirror(bool i){this->_mirror = i;}
    void setRate(int i){this->_rota = i;}

signals:
    void readReady(QImage);
    void readReadyMat(cv::Mat);
    void getFrame(cv::Mat*);
private slots:
    void readFrame(cv::Mat*);
private:
    QThread *thread;
    // 设置输出的视频信号是否裁切
    bool _cutvideo;
    cv::Rect _cutRect;
    cv::VideoCapture *_cap;
    // Double buffering
    cv::Mat _DB_frame[2];
//    cv::Mat _DB_frame_list;
    int _DB_ind;
    QTimer _timer;
    //
    bool _mirror;
    int _rota;
};

#endif // MY_CAMERA_OCV_H
