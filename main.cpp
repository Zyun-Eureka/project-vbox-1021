#include "widget.h"
#include <QApplication>
#include "my_close.h"
#include "funlist.h"
#include <QDebug>

//#include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    my_close *mc = new my_close();
    a.installEventFilter(mc);
    Widget *w = new Widget();
    mc->add(w);
    w->show();

    return a.exec();
}
