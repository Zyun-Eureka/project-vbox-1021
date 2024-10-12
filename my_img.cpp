#include "my_img.h"
#include <QDebug>
#include <QThread>

my_img::my_img(QObject *parent)
    : QObject{parent}
{
    click = false;
    wi = nullptr;
}

void my_img::clicked()
{
    emit _sig_clicked(this);
}

void my_img::_slot_clicked(const my_img *p)
{
    if(wi==nullptr)return;
    if(p==this){
        click = !click;
        wi->update();
    }else{
        if(click){
            click = false;
            wi->update();
        }
    }
}
