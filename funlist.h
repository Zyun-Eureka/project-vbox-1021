#ifndef FUNLIST_H
#define FUNLIST_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QEvent>

#include <QStandardItemModel>
#include <QStandardItem>

#include <QTimer>

#include <QFile>
#include <QRandomGenerator>


#define COLUMN 3

namespace Ui {
class funlist;
}

class funlist : public QWidget
{
    Q_OBJECT

public:
    explicit funlist(QWidget *parent = nullptr);
    ~funlist();

    QPushButton* addbutton(QString name);
    bool eventFilter(QObject *watched, QEvent *event);
    void addvalue(int index);

signals:
    void listShow(bool);

    void setting(int,bool,double);

private slots:
    void on_cutBox_released();

    void on_radius_currentIndexChanged(int index);

    void on_mirror_toggled(bool checked);

    void on_startopc_toggled(bool checked);

    void on_fps_valueChanged(int arg1);

    void on_enable_cut_toggled(bool checked);

private:

    void initTable();
    void addItemT(QString name);

    QSize bts;
    QWidget *_widget;
    QGridLayout *_layout;
    QList<QPushButton*> _btLists;
    QList<int> _tableValues;
    QStandardItemModel *_tableModel;

    Ui::funlist *ui;
};

#endif // FUNLIST_H
