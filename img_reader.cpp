#include "img_reader.h"

#include <QDebug>

img_reader::img_reader(QObject *parent)
    : QObject{parent}
{
    _idir.setPath(QApplication::applicationDirPath());
    if(!_idir.exists("input")){
        _idir.mkdir("input");
    }
    if(!_idir.exists("output")){
        _idir.mkdir("output");
    }
    _odir.setPath(_idir.path());
    _idir.cd("input");
    _odir.cd("output");

    filter <<"*.jpg"<<"*.png"<<"*.jpeg";
    index = 0;
    state = false;

    connect(this,SIGNAL(run()),this,SLOT(loop()));

    _sig_thread = new sig_thread();
}

img_reader::~img_reader()
{
    delete _sig_thread;
    state = false;
}

void img_reader::setnum(int i)
{
    num = i;
    my_img *y;
    while (img_lists.count()<num) {
        y = new my_img();
        connect(y,SIGNAL(_sig_clicked(const my_img*)),_sig_thread,SIGNAL(_sig_img_change(const my_img*)));
        connect(_sig_thread,SIGNAL(_sig_img_change(const my_img*)),y,SLOT(_slot_clicked(const my_img*)));
        img_lists.push_back(y);
    }
}

void img_reader::start()
{
    state = true;
    emit run();
}

void img_reader::stop()
{
    state = false;
}

QList<my_img*> *img_reader::getList()
{
    return &img_lists;
}

void img_reader::c_index(int i)
{
    cindex = i;
}

void img_reader::loop()
{
    while (state) {
        plist = _idir.entryList(filter,QDir::NoDotAndDotDot|QDir::Files,QDir::Time);
        if(plist.isEmpty()){
            thread()->sleep(1);
        }
        for(QString i:plist){
            _file.setFileName(QString("%1/%2").arg(_idir.path()).arg(i));
            img_lists[index]->img = QImage(_file.fileName());
            emit readReady(index);
            _file.rename(QString("%1/[%2]%3").arg(_odir.path()).arg(QDateTime::currentDateTime().toString("yyddMMHHmmss")).arg(i));
            thread()->msleep(200);
            if(cindex!=-1){
                cindex++;
                if(cindex == num){
                    cindex = -1;
                }
                emit click_index(cindex);
            }
            index++;
            if(index == num){
                index = 0;
            }
        }
        plist.clear();
    }
}
