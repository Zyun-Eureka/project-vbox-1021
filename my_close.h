#ifndef MY_CLOSE_H
#define MY_CLOSE_H

#include <QObject>
#include <QEvent>
#include "widget.h"

class my_close : public QObject
{
    Q_OBJECT
public:
    explicit my_close(QObject *parent = nullptr);

    void add(Widget *o);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Widget *w;
};

#endif // MY_CLOSE_H
