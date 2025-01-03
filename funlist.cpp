#include "funlist.h"
#include "ui_funlist.h"

#include <QDebug>

funlist::funlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::funlist)
{
    ui->setupUi(this);

    //
    _widget = new QWidget();
    _layout = new QGridLayout();

    _widget->setLayout(_layout);
    _layout->setSpacing(0);
    _layout->setMargin(0);
    _layout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    ui->buttonArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->buttonArea->setWidget(_widget);
    ui->buttonArea->setWidgetResizable(true);
    bts = QSize(1,1);

    initTable();

    QFile file;
    file.setFileName(QApplication::applicationDirPath()+"/Keys.txt");
    file.open(QIODevice::ReadWrite);
    QByteArray byte = file.readLine();
    while (!byte.isEmpty()) {
        if(byte.back()=='\n'){
            byte = byte.mid(0,byte.length()-2);
        }
        addbutton(QString::fromUtf8(byte));
        byte = file.readLine();
    }

    file.close();

    ui->return_lab->installEventFilter(this);
    ui->buttonArea->installEventFilter(this);


    ui->system_lab->installEventFilter(this);
    ui->setting_lab->installEventFilter(this);
    ui->count_lab->installEventFilter(this);
}

funlist::~funlist()
{
    delete ui;
}

QPushButton *funlist::addbutton(QString name)
{
    QPushButton *bt = new QPushButton();
    bt->setText(name);
    bt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    bt->setCheckable(true);
    QLabel l;
    l.setWordWrap(true);
    if(_btLists.count()!=0){
        _layout->addWidget(bt,_btLists.count()/COLUMN,_btLists.count()%COLUMN);
    }else{
        _layout->addWidget(bt,0,0);
    }
    _btLists.push_back(bt);
    bt->setMaximumSize(bts);
    bt->setMinimumHeight(bts.height());
    addItemT(name);
    return bt;
}

bool funlist::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->buttonArea){
        if(event->type()==QEvent::Resize){
            bts = ui->buttonArea->size()/COLUMN;
            bts.setHeight( bts.width());
            for(QPushButton* bt:_btLists){
                bt->setMaximumSize(bts);
                bt->setMinimumHeight(bts.height());
            }
        }
    }else if(watched == ui->return_lab){
        if(event->type()==QEvent::MouseButtonRelease){
            emit listShow(false);
            ui->icon->setCurrentIndex(1);
        }
    }else if(watched == ui->count_lab){
        if(event->type()==QEvent::MouseButtonRelease){
            emit listShow(true);
            ui->icon->setCurrentIndex(0);
            ui->mainpage->setCurrentIndex(0);
        }
    }else if(watched == ui->setting_lab){
        if(event->type()==QEvent::MouseButtonRelease){
            emit listShow(true);
            ui->icon->setCurrentIndex(0);
            ui->mainpage->setCurrentIndex(1);
        }
    }
    else if(watched == ui->system_lab){
        if(event->type()==QEvent::MouseButtonRelease){
            emit listShow(true);
            ui->icon->setCurrentIndex(0);
            ui->mainpage->setCurrentIndex(2);
        }
    }
    return QWidget::eventFilter(watched,event);;
}

void funlist::addvalue(int index)
{
    if(index>=_tableModel->rowCount()||index<0)return;
    _tableValues[index]++;
    _tableModel->item(index,1)->setText(QString::number(_tableValues[index]));
}

void funlist::initTable()
{
    _tableModel = new QStandardItemModel(0,2);
    ui->tables->setModel(_tableModel);
    ui->tables->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _tableModel->setHorizontalHeaderLabels({"物品名称","检出数量"});
}

void funlist::addItemT(QString name)
{
    QStandardItem *item_s = new QStandardItem("0");
    QStandardItem *item = new QStandardItem(name);
    item->setTextAlignment(Qt::AlignCenter);
    item_s->setTextAlignment(Qt::AlignCenter);
    _tableModel->appendRow({item,item_s});
    _tableValues.push_back(0);
}

void funlist::on_cutBox_released()
{
    emit setting(1,true,0);
}


void funlist::on_radius_currentIndexChanged(int index)
{
    emit setting(3,true,index);
}


void funlist::on_mirror_toggled(bool checked)
{
    emit setting(2,checked,0);
    if(checked){
        ui->mirror->setText("关闭");
    }else{
        ui->mirror->setText("启用");
    }
}


void funlist::on_startopc_toggled(bool checked)
{
    emit setting(4,checked,0);
    if(checked){
        ui->startopc->setText("关闭");
    }else{
        ui->startopc->setText("启用");
    }
}


void funlist::on_fps_valueChanged(int arg1)
{
    emit setting(5,true,arg1);
}

void funlist::on_enable_cut_toggled(bool checked)
{
    if(checked){
        ui->enable_cut->setText("停止裁切");
    }else{
        ui->enable_cut->setText("启用裁切");
    }
    emit setting(6,checked,0);
}

