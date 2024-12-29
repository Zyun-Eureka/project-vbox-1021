#include "my_cut_config.h"
#include "ui_my_cut_config.h"
#include <QDebug>

my_cut_config::my_cut_config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::my_cut_config)
{
    ui->setupUi(this);
    setWindowTitle("裁切设置");
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
}

my_cut_config::~my_cut_config()
{
    delete ui;
}

void my_cut_config::on_apply_released()
{
    emit cutRect(ui->px->value(),ui->py->value(),ui->pw->value(),ui->ph->value());
}


void my_cut_config::on_config_released()
{
    close();
}
