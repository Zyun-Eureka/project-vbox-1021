#ifndef MY_CUT_CONFIG_H
#define MY_CUT_CONFIG_H

#include <QDialog>

namespace Ui {
class my_cut_config;
}

class my_cut_config : public QDialog
{
    Q_OBJECT

public:
    explicit my_cut_config(QWidget *parent = nullptr);
    ~my_cut_config();
signals:
    void cutRect(int x,int y,int width,int height);
private slots:
    void on_apply_released();

    void on_config_released();

private:
    Ui::my_cut_config *ui;
};

#endif // MY_CUT_CONFIG_H
