#ifndef IMG_READER_H
#define IMG_READER_H

#include <QObject>

#include <QApplication>
#include <QDir>
#include <QFileSystemWatcher>

#include <QTimer>

#include <QImage>
#include <QFile>

#include <QThread>
#include <QDateTime>

class img_reader : public QObject
{
    Q_OBJECT
public:
    explicit img_reader(QObject *parent = nullptr);
    ~img_reader();


    void setnum(int i);

    void start();
    void stop();

signals:
    void readReady(int head_i,QList<QImage> *list);
    void run();
private slots:
    void loop();
private:
    bool state;


    QList<QImage> img_lists;
    QStringList filter;
    QStringList plist;
    QDir _idir;
    QDir _odir;
    QFile _file;
    int num;

    int index;

};

#endif // IMG_READER_H
