#include "img_reader.h"

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
}

img_reader::~img_reader()
{
    state = false;
}

void img_reader::setnum(int i)
{
    num = i;
    while (img_lists.count()<num) {
        img_lists.push_back(QImage());
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

void img_reader::loop()
{
    plist = _idir.entryList(filter,QDir::NoDotAndDotDot|QDir::Files,QDir::Time);
    if(plist.isEmpty()){
        thread()->sleep(1);
    }
    for(QString i:plist){
        _file.setFileName(QString("%1/%2").arg(_idir.path()).arg(i));
        img_lists[index] = QImage(_file.fileName());
        emit readReady(index,&img_lists);
        _file.rename(QString("%1/[%2]%3").arg(_odir.path()).arg(QDateTime::currentDateTime().toString("yyddMMHHmmss")).arg(i));
        thread()->msleep(200);
        index++;
        if(index == num){
            index = 0;
        }
    }
    plist.clear();
    if(state)loop();
}
