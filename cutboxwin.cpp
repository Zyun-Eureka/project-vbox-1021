#include "cutboxwin.h"
#include "ui_cutboxwin.h"
#include <QDebug>

CutBoxWin::CutBoxWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutBoxWin)
{
    ui->setupUi(this);

    setWindowTitle("窗口裁切");
    //pen init
    _cutbox_pen.setColor(Qt::red);

    ui->cutBox->installEventFilter(this);
    cutRect.setX(-1);
}

CutBoxWin::~CutBoxWin()
{
    delete ui;
}

bool CutBoxWin::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->cutBox){
        if(event->type()==QEvent::Paint){
            QPainter pa(ui->cutBox);
            pa.setPen(_cutbox_pen);
            pa.drawRect(0,0,_cut_pw,_cut_ph);
            pa.setBrush(Qt::red);
            pa.drawEllipse(_cut_pw-CUTBOX_ELLIPSE_REDIUS,_cut_ph-CUTBOX_ELLIPSE_REDIUS,CUTBOX_ELLIPSE_WIDTH,CUTBOX_ELLIPSE_WIDTH);
        }else if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *m = static_cast<QMouseEvent*>(event);
            if(QRect(_cut_pw-CUTBOX_ELLIPSE_REDIUS,_cut_ph-CUTBOX_ELLIPSE_REDIUS,CUTBOX_ELLIPSE_WIDTH,CUTBOX_ELLIPSE_WIDTH).contains(m->pos())){
                _cut_flage = 1;
            }else if(QRect(_cut_pw-CUTBOX_ELLIPSE_REDIUS,0,CUTBOX_ELLIPSE_WIDTH,_cut_ph).contains(m->pos())){
                _cut_flage = 2;
            }else if(QRect(0,_cut_ph-CUTBOX_ELLIPSE_REDIUS,_cut_pw,CUTBOX_ELLIPSE_WIDTH).contains(m->pos())){
                _cut_flage = 3;
            }else{
                _cut_point_flage = m->pos();
                _cut_flage = 4;
            }
        }else if(event->type()==QEvent::Resize){
            if(ui->cutBox->width()<100){
                ui->cutBox->setGeometry(ui->cutBox->x(),ui->cutBox->y(),100,ui->cutBox->height());
            }
            if(ui->cutBox->height()<100){
                ui->cutBox->setGeometry(ui->cutBox->x(),ui->cutBox->y(),ui->cutBox->width(),100);
            }
            _cut_pw = ui->cutBox->width()-_cutbox_pen.width()-CUTBOX_ELLIPSE_REDIUS;
            _cut_ph = ui->cutBox->height()-_cutbox_pen.width()-CUTBOX_ELLIPSE_REDIUS;
        }else if(event->type()==QEvent::MouseMove){
            QMouseEvent *m = static_cast<QMouseEvent*>(event);
            switch (_cut_flage) {
            case 1:
                _cut_point_flage.setY(m->pos().y()+CUTBOX_ELLIPSE_REDIUS);
                ui->cutBox->setGeometry(ui->cutBox->x(),ui->cutBox->y(),ui->cutBox->width(),(_cut_point_flage.y()+ui->cutBox->y()-CUTBOX_ELLIPSE_REDIUS)>height()?ui->cutBox->height():_cut_point_flage.y());
            case 2:
                _cut_point_flage.setX(m->pos().x()+CUTBOX_ELLIPSE_REDIUS);
                ui->cutBox->setGeometry(ui->cutBox->x(),ui->cutBox->y(),(_cut_point_flage.x()+ui->cutBox->x()-CUTBOX_ELLIPSE_REDIUS)>width()?ui->cutBox->width():_cut_point_flage.x(),ui->cutBox->height());
                break;
            case 3:
                _cut_point_flage.setY(m->pos().y()+CUTBOX_ELLIPSE_REDIUS);
                ui->cutBox->setGeometry(ui->cutBox->x(),ui->cutBox->y(),ui->cutBox->width(),(_cut_point_flage.y()+ui->cutBox->y()-CUTBOX_ELLIPSE_REDIUS)>height()?ui->cutBox->height():_cut_point_flage.y());
                break;
            case 4:
                ui->cutBox->move(ui->cutBox->pos()+m->pos()-_cut_point_flage);
                break;
            default:
                break;
            }
            if(ui->cutBox->pos().x()<0){
                ui->cutBox->move(0,ui->cutBox->y());
            }
            if(ui->cutBox->pos().y()<0){
                ui->cutBox->move(ui->cutBox->x(),0);
            }
            if(ui->cutBox->pos().x()+ui->cutBox->width()>width()+CUTBOX_ELLIPSE_REDIUS){
                ui->cutBox->move(width()-ui->cutBox->width()+CUTBOX_ELLIPSE_REDIUS,ui->cutBox->y());
            }
            if(ui->cutBox->pos().y()+ui->cutBox->height()>height()+CUTBOX_ELLIPSE_REDIUS){
                ui->cutBox->move(ui->cutBox->x(),height()-ui->cutBox->height()+CUTBOX_ELLIPSE_REDIUS);
            }
        }else if(event->type()==QEvent::MouseButtonRelease||event->type()==QEvent::Show){
            cutRect.setWidth(ui->cutBox->width());
            cutRect.setHeight(ui->cutBox->height());
            cutRect.setX(ui->cutBox->x());
            cutRect.setY(ui->cutBox->y());
        }
    }
    return QWidget::eventFilter(watched,event);
}
