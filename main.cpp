#include "widget.h"
#include <QApplication>
#include <VLCQtCore/Common.h>
#include "my_close.h"
#include "funlist.h"

#include "mycamera.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    VlcCommon::setPluginPath(a.applicationDirPath() + "/plugins");
//    my_close *mc = new my_close();
//    a.installEventFilter(mc);
//    Widget *w = new Widget();
//    mc->add(w);
//    w->show();

    myCamera my;
    my.show();
    return a.exec();
}
