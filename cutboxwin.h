#ifndef CUTBOXWIN_H
#define CUTBOXWIN_H

#include <QWidget>

#include <QPainter>
#include <QEvent>
#include <QMouseEvent>

#define CUTBOX_ELLIPSE_REDIUS 3
#define CUTBOX_ELLIPSE_WIDTH 6

namespace Ui {
class CutBoxWin;
}

class CutBoxWin : public QWidget
{
    Q_OBJECT

public:
    explicit CutBoxWin(QWidget *parent = nullptr);
    ~CutBoxWin();
    bool eventFilter(QObject *watched, QEvent *event);

    QRect cutRect;
    QImage img;
    QImage Frame;



public slots:
    void drawvideo(QImage);

private:
    QRect _video_rect;
    QPoint _cut_point_flage;
    double _cut_pw,_cut_ph;
    QRect _cutbox_rect;
    QPen pen;
    QPen _cutbox_pen;
    int _cut_flage;
    Ui::CutBoxWin *ui;
};

#endif // CUTBOXWIN_H
