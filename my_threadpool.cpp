#include "my_threadpool.h"
#include <QDebug>

my_threadpool::my_threadpool(QObject *parent)
    : QObject{parent}
{
    thread = new QThread();
    moveToThread(thread);
    thread->start();

    _mytf_latest = new my_transform(nullptr);
    _mytf_latest->next = _mytf_latest;
    _mytf_list.push_back(_mytf_latest);
}

my_threadpool::~my_threadpool()
{
    foreach (my_transform *i, _mytf_list) {
        i->thread()->quit();
        i->thread()->wait();
        delete i;
    }
    thread->quit();
    thread->wait();
    delete thread;
}

void my_threadpool::getImage(QImage i)
{
//    qDebug()<<"get";
    _mytf_now = _mytf_latest;
    while (true) {
        if(_mytf_now->mutex.tryLock(0)){
            _mytf_now->run(i);
//            qDebug()<<_mytf_now->thread()->currentThreadId()<<thread()->currentThread();
            _mytf_latest = _mytf_now->next;
            break;
        }else{
            if(_mytf_now->next==_mytf_latest){
                if(_mytf_list.length()>=10)continue;
                _mytf_now = new my_transform(_mytf_list.first());
                connect(_mytf_now,&my_transform::ready,[=](){
                    this->emit ready(_mytf_now);
                });
                _mytf_list.last()->next = _mytf_now;
                _mytf_list.push_back(_mytf_now);

            }else{
                _mytf_now = _mytf_now->next;
            }
        }
    }
}
