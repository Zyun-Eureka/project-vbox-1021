#include "my_close.h"
#include <QMessageBox>

my_close::my_close(QObject *parent)
    : QObject{parent}
{

}

void my_close::add(Widget *o)
{
    w = o;
}

bool my_close::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::Quit){
        w->_deleteBefore();
        delete w;
        delete this;
        event->accept();
    }
    return QObject::eventFilter(watched,event);;
}
