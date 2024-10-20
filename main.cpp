#include "widget.h"
#include <QApplication>
#include "my_close.h"
#include "funlist.h"

//#include "mycamera.h"

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
