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

    ui->buttonArea->installEventFilter(this);
    this->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    timer->start(200);
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
    }if(watched == this){
        if(event->type()==QEvent::Enter){
//            qDebug()<<"2";
            ui->icon->hide();
        }else if(event->type()==QEvent::Leave){
//            qDebug()<<"2";
            ui->icon->show();
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

void funlist::timeOut()
{
    if(ui->checkBox->isChecked()){
        addvalue(QRandomGenerator::global()->generate()%_tableModel->rowCount());
    }
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
    QStandardItem *item,*item_s = new QStandardItem("0");
    item = new QStandardItem(name);
    item->setTextAlignment(Qt::AlignCenter);
    item_s->setTextAlignment(Qt::AlignCenter);
    _tableModel->appendRow({item,item_s});
    _tableValues.push_back(0);
}
