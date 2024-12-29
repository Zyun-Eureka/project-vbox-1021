#include "widget.h"
#include <QApplication>
#include "my_close.h"
#include "funlist.h"
#include <QDebug>

//#include "my_cut_config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    my_camera_ocv my;
//    CutBoxWin c;
//    QObject::connect(&my,SIGNAL(readReady(QImage)),&c,SLOT(drawvideo(QImage)));
////    c.show();
//    QWidget w;
//    w.show();
//    QPushButton b(&w);
//    b.show();
//    QObject::connect(&b,&QPushButton::clicked,[&](){
//        (&c)->show();
//    });
//    my.cutRect(10,10,300,300);
//    my.cutOut();
//    c.show();

    my_close *mc = new my_close();
    a.installEventFilter(mc);
    Widget *w = new Widget();
    mc->add(w);
    w->show();
//    my_cut_config cc;
//    cc.show();
    return a.exec();
}
